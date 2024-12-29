#include "minishell.h"

char	**g_env = NULL;

int	has_pipe(t_word *args)
{
	while (args)
	{
		if (args->type == PIPE)
			return (1);
		args = args->next;
	}
	return (0);
}

void	ft_print_error(int i)
{
	static int	status;
	char		*err_msg[7];

	err_msg[0] = " command not found\n";
	err_msg[1] = " too many arguments\n";
	err_msg[2] = " numeric argument required\n";
	err_msg[3] = " not a valid identifier\n";
	err_msg[4] = " No such file or directory\n";
	err_msg[5] = " Permission denied\n";
	err_msg[6] = "minishell: syntax error near unexpected token `newline'\n";
	if (!status)
	{
		status = 1;
		if (i == -1)
		{
			ft_printf_fd(STDERR_FILENO, "");
		}
		else
			ft_printf_fd(STDERR_FILENO, err_msg[i]);
	}
}

const char	*token_type_to_str(t_tokens type)
{
	switch (type)
	{
		case COMMAND:
			return ("COMMAND");
		case ARGUMENT:
			return ("ARGUMENT");
		case PIPE:
			return ("PIPE");
		case REDIRECT_IN:
			return ("REDIRECT_IN");
		case REDIRECT_OUT:
			return ("REDIRECT_OUT");
		case END:
			return ("END");
		case HEREDOC:
			return ("HEREDOC");
		case REDIRECT_APPEND:
			return ("REDIRECT_APPEND");
		default :
			return ("UNKNOWN");
	}
}

int	check_path(const char *path, char ***envp)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
	{
		if (errno == EACCES)
		{
			fprintf(stderr, "Error: %s: Permission denied\n", path);
		}
		else
		{
			ft_printf_fd(STDERR_FILENO, " %s\n", strerror(errno));
			ft_put_exitcode(envp, 127);
		}
		return (0);
	}
	if (access(path, R_OK | W_OK | X_OK | F_OK) == -1)
	{
		ft_printf_fd(STDERR_FILENO, " Permission denied\n");
		ft_put_exitcode(envp, 126);
		return (0);
	}
	if (S_ISREG(path_stat.st_mode))
	{
		return (1);
	}
	else if (S_ISDIR(path_stat.st_mode))
	{
		ft_printf_fd(STDERR_FILENO, " Is a directory\n");
		ft_put_exitcode(envp, 126);
		return (0);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, " Is neither a file nor a directory.\n");
		ft_put_exitcode(envp, 127);
		return (0);
	}
	return (1);
}

int	ft_extras(char **word, t_word **args, char ***envp)
{
	if (!ft_strncmp((*args)->value, "$PWD", 4))
	{
		ft_printf_fd(STDERR_FILENO, " Is a directory\n");
		ft_put_exitcode(envp, 126);
		return (0);
	}
	else if (!ft_strncmp((*args)->value, "$", 2)
		|| !ft_strncmp((*args)->value, "$?", 3))
	{
		ft_printf_fd(STDERR_FILENO, " command not found\n");
		ft_put_exitcode(envp, 127);
		return (0);
	}
	else if (!ft_strncmp((*args)->value, "$EMPTY", 6))
	{
		if (ft_strncmp((*args)->next->value, "END", 3))
		{
			*args = (*args)->next;
			*word = (*args)->value;
			return (1);
		}
		ft_put_exitcode(envp, 0);
		return (0);
	}
	else if (!ft_strncmp((*args)->value, "/", 1)
		|| !ft_strncmp((*args)->value, "./", 2))
	{
		if (!check_path(*word, envp))
			return (0);
	}
	return (1);
}

int	is_bt(char *word, t_word *args, char ***envp)
{
	if (!ft_extras(&word, &args, envp))
		return (0);
	if (!ft_strncmp(word, "echo", 5))
		return (bt_echo(args, 1, envp));
	if (!ft_strncmp(word, "cd", 3))
		return (bt_cd(args, envp));
	if (!ft_strncmp(word, "pwd", 4))
		return (bt_pwd());
	if (!ft_strncmp(word, "export", 7))
		return (bt_export(args, envp));
	if (!ft_strncmp(word, "unset", 6))
		return (bt_unset(args, envp));
	if (!ft_strncmp(word, "env", 4))
		return (bt_env(*envp));
	if (!ft_strncmp(word, "exit", 5))
		return (bt_exit(args, envp));
	return (1);
}

void	ft_init_shelly(t_shelly **mini)
{
	*mini = malloc(sizeof(t_shelly));
	if (!(*mini))
		return ;
	(*mini)->exit_code = 1;
	(*mini)->i = -1;
	(*mini)->j = -1;
	(*mini)->k = -1;
	(*mini)->l = -1;
}

int	main(int argc, char **argv, char **envp)
{
	char	***my_env;
	t_word	*temp;
	t_word	*args;
	t_word	*next;
	char	*line;

	argc = 0;
	argv = NULL;
	my_env = env_init(envp);
	signal(SIGINT, ft_handlesignal);
	signal(SIGQUIT, SIG_IGN);
	args = NULL;
	line = NULL;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
		{
			ft_printf("exit\n");
			ft_free_all(my_env, &args);
			break ;
		}
		if (strlen(line) > 0)
			add_history(line);
		if (*line && *line != ' ')
		{
			lexer(line, &args);
			temp = args;
			while (temp)
			{
				temp = temp->next;
			}
			if (has_pipe(args))
			{
				pipe_execution(args, my_env);
			}
			else if (is_bt(args->value, args, my_env))
			{
				ft_auto_execute(args, my_env);
			}
			free(line);
		}
		if (args)
		{
			while (args)
			{
				next = args->next;
				free(args->value);
				free(args);
				args = next;
			}
		}
	}
	return (0);
}
