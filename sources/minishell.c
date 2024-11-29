#include "minishell.h"


char	**g_env = NULL;

int has_pipe(t_word *args)
{
	while (args)
	{
		if (args->type == PIPE)
			return (1);
		args = args->next;
	}
	return (0);
}

void ft_print_error(int i)
{
	static int	status;
	char		*err_msg[6];

	//ft_printf("STATUS BEFORE: %s\n", status);
	if (!status)
		status = 1;
	else if (status == 1)
		return ;
	//ft_printf("STATUS AFTER: %s\n", status);
	err_msg[0] = " command not found\n";
	err_msg[1] = " too many arguments\n";
	err_msg[2] = " numeric argument required\n";
	err_msg[3] = " not a valid identifier\n";
	err_msg[4] = " No such file or directory\n";
	err_msg[5] = " Permission denied\n";

	if (i == -1)
		ft_printf_fd(STDERR_FILENO, "\n");
	else
		ft_printf_fd(STDERR_FILENO, err_msg[i]);
}

const char *token_type_to_str(t_tokens type)
{
	switch (type)
	{
		case COMMAND:
			return "COMMAND";
		case ARGUMENT:
			return "ARGUMENT";
	//        case QUOTE:
	//           return "QUOTE";
		/* case VARIABLE:
			return "VARIABLE"; */
		case PIPE:
			return "PIPE";
		case REDIRECT_IN:
			return "REDIRECT_IN";
		case REDIRECT_OUT:
			return "REDIRECT_OUT";
		case END:
			return "END";
		case HEREDOC:
			return "HEREDOC";
		case REDIRECT_APPEND:
			return "REDIRECT_APPEND";
		default:
			return "UNKNOWN";
	}
}
int check_path(const char *path,  char ***envp) {
    struct stat path_stat;

    // Call stat to get the file information
    if (stat(path, &path_stat) == -1) {
        // Print an error if the path is invalid
		if (errno == EACCES) {
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
    // Check the type of the file
    if (S_ISREG(path_stat.st_mode)) {
        return (1);//ft_printf_fd(STDERR_FILENO, "%s is a regular file.\n", path);
    } else if (S_ISDIR(path_stat.st_mode)) {
        ft_printf_fd(STDERR_FILENO, " Is a directory\n");
		ft_put_exitcode(envp, 126);
		return (0);
    } else {
        ft_printf_fd(STDERR_FILENO, " Is neither a file nor a directory.\n");
		ft_put_exitcode(envp, 127);
		return (0);
	}
	return (1);
}

int ft_extras(char **word, t_word **args, char ***envp)
{
	//struct 	stat path_stat;
	//int		file_stat;

	/* file_stat = stat(*word, &path_stat);
	if (file_stat == -1)
	{
		if(ft_strcmp(strerror(errno), " No such file or directory"))
		{
			ft_printf_fd(STDERR_FILENO, " %s\n", strerror(errno));
			ft_put_exitcode(envp, 127);
			return (0);
		}
	} */

	//ft_printf("Value: %d", stat((*args)->value, &path_stat));
	/* if (!ft_strncmp((*args)->value, "/", 1) || !ft_strncmp((*args)->value, "./", 2) || ft_strchr((*args)->value, '.'))
	{
		if (stat((*args)->value, &path_stat) == -1)
		{
			if (!ft_strncmp((*args)->value, "/", 1) || !ft_strncmp((*args)->value, "./", 2))
				ft_printf_fd(STDERR_FILENO, " %s\n", strerror(errno));
			else if (ft_strchr((*args)->value, '.'))
				ft_printf_fd(STDERR_FILENO, " command not found\n");
			ft_put_exitcode(envp, 127);
			return (0);
		}
	} */
	if (!ft_strncmp((*args)->value, "$PWD", 4))
	{
		ft_printf_fd(STDERR_FILENO, " Is a directory\n");
		ft_put_exitcode(envp, 126);
		return (0);
	}
	else if (!ft_strncmp((*args)->value, "$", 2) || !ft_strncmp((*args)->value, "$?", 3))
	{
		ft_printf_fd(STDERR_FILENO, " command not found\n");
		ft_put_exitcode(envp, 127);
		return (0);
	}
	else if(!ft_strncmp((*args)->value, "$EMPTY", 6))
	{
		//ft_printf("VALUE: %s\n", args->next->value);
		if (ft_strncmp((*args)->next->value, "END", 3))
		{
			*args = (*args)->next;
			*word = (*args)->value;
			//ft_printf("VALUE BEFORE: %s\n", (*args)->value);
			return (1);
		}
		ft_put_exitcode(envp, 0);
		return (0);
	}
	else if (!ft_strncmp((*args)->value, "/", 1) || !ft_strncmp((*args)->value, "./", 2) /* || ft_strchr((*args)->value, '.') */)
	{
		if (!check_path(*word, envp))
			return (0);
	}
	/* if (stat((*args)->value, &path_stat) == -1)
	{
		//ft_printf_fd(STDERR_FILENO, " %s\n", strerror(errno));
		ft_put_exitcode(envp, 127);
		return (0);
	} */
	return (1);
}


int is_bt(char *word, t_word *args, char ***envp)
{
	if (!ft_extras(&word, &args, envp))
		return (0);
	//ft_printf("VALUE AFTER: %s\n", (args)->value);	
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

void ft_init_shelly(t_shelly **mini)
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

int main(int argc, char **argv, char **envp)
{
	t_shelly **mini;
    char ***my_env;

	argc = 0;
    argv = NULL;
	mini = malloc(sizeof(t_shelly));
	my_env = env_init(envp);
	ft_init_shelly(mini);
	//printf("VALUE %d\n", (*mini)->exit_code);
    signal(SIGINT, ft_handlesignal);
    signal(SIGQUIT, SIG_IGN);
    char *line = NULL;
    t_word *args = NULL;

    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL) {
			//ft_printf("VALUE before: %s\n", args->value);
            ft_printf("exit\n");
			ft_free_all(my_env, &args);
			//exit (0);
            break;
        }
        if (strlen(line) > 0)
            add_history(line);


        if (*line && *line != ' ')
        {
            lexer(line, &args);
			t_word *temp = args;
			while (temp)
			{
				//printf("Token Type: %s, Value: %s\n", token_type_to_str(temp->type), temp->value);
				temp = temp->next;
			}
			//printf("line 		: %s\n", line);
            if (has_pipe(args)) {
                pipe_execution(args, my_env);
            } else if (is_bt(args->value, args, my_env)) {
                ft_auto_execute(args, my_env);
            }
        	free(line);
			//ft_free_args(args);
        }
		//ft_free_args(args);
        while (args)
		{
			t_word *next = args->next;
			free(args->value);
			free(args);
			args = next;
		}
    }
    return (0);
}
// =======
// 	argc = 0;
// 	argv = NULL;
// 	my_env = env_init(envp);
// 	if (!my_env)
// 		return (0);
// 	signal(SIGINT, ft_handlesignal);
// 	signal(SIGQUIT, SIG_IGN);
// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (line == NULL)
// 		{
// 			ft_printf("exit\n");
// 			ft_free_all(my_env, &args);
// 			//printf("FINISHED FREE\n");
// 			break ;
// 		}
// 		if (strlen(line) > 0)
// 			add_history(line);
// 		if (line)
// 		{
// 			lexer(line, &args);
// 			temp = args;
// 			while (temp)
// 			{
// 				printf("Token Type: %s, Value: %s\n",
// 					token_type_to_str(temp->type), temp->value);
// 				temp = temp->next;
// 			}
// 		}
// 		/* if (strcmp(line, "clear") == 0)
// 		{
// 			system("clear");
//     		return (0);
// 		} */
// 			//ft_clear_screen();
// 		if (is_bt(args->value, args, &my_env))
// 		{
// 			ft_auto_execute(args);
// 		}
// 		free(line);
// 		while (args)
// 		{
// 			next = args->next;
// 			free(args->value);
// 			free(args);
// 			args = next;
// 		}
// 	}
// 	return (0);
// >>>>>>> 8cb5b18e94af2b779b7dd5fd01b5608b698c7b3f
// }

// =======

// int main(int argc, char **argv, char **envp)
// {
// 	char	**my_env;
// 	t_word	*next;
// 	char	*line = NULL;
// 	t_word	*args = NULL;
// 	t_word	*temp;

// 	argc = 0;
// 	argv = NULL;
// 	my_env = env_init(envp);
// 	if (!my_env)
// 		return (0);
// 	signal(SIGINT, ft_handlesignal);
// 	signal(SIGQUIT, SIG_IGN);
// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (line == NULL)
// 		{
// 			ft_printf("exit\n");
// 			ft_free_all(my_env, &args);
// 			break ;
// 		}
// 		if (strlen(line) > 0)
// 			add_history(line);
// 		if (line)
// 		{
// 			lexer(line, &args);
// 			temp = args;
// 			while (temp)
// 			{
// 				printf("Token Type: %s, Value: %s\n",
// 					token_type_to_str(temp->type), temp->value);
// 				temp = temp->next;
// 			}
// 		}
// 		if (strcmp(line, "clear") == 0)
// 			ft_clear_screen();
// 		if (is_bt(args->value, args, &my_env))
// 		{
// 			ft_auto_execute(args);
// 		}
// 		free(line);
// 		while (args)
// 		{
// 			next = args->next;
// 			free(args->value);
// 			free(args);
// 			args = next;
// 		}
// 	}
// 	return (0);
// >>>>>>> origin/peferrei

