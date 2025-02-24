#include "minishell.h"

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
	char		*err_msg[12];

	err_msg[0] = " command not found\n";
	err_msg[1] = " too many arguments\n";
	err_msg[2] = " numeric argument required\n";
	err_msg[3] = " not a valid identifier\n";
	err_msg[4] = " No such file or directory\n";
	err_msg[5] = " Permission denied\n";
	err_msg[6] = "minishell: syntax error near unexpected token `newline'\n";
	err_msg[7] = " Is a directory\n";
	err_msg[8] = " Is a directory\n";
	err_msg[9] = " Arguments and options aren't supported\n";
	err_msg[10] = " Options aren't supported\n";
	err_msg[11] = " Syntax error\n";
	if (!status)
	{
		status = 1;
		if (i == -1)
			ft_printf_fd(STDERR_FILENO, "");
		else
			ft_printf_fd(STDERR_FILENO, err_msg[i]);
	}
	if (i == -1)
		status = 0;
}

const char	*token_type_to_str(t_tokens type)
{
	if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == END)
		return ("END");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else
		return ("UNKNOWN");
}

int	ft_belong_env(const char *path)
{
	char	**str;
	char	*str_env;

	str = ft_split(path, '/');
	str_env = getenv("PATH");
	if (ft_strnstr(str_env, str[0], ft_strlen(str_env)))
	{
		ft_free_argvs(str);
		return (1);
	}
	ft_free_argvs(str);
	return (0);
}

int	check_path(char *path, char ***envp)
{
	struct stat	path_stat;

	if (ft_belong_env(path) == 1)
		return (1);
	if (stat(path, &path_stat) == -1)
	{
		if (errno == EACCES)
			ft_print_error(5);
		else
		{
			ft_print_error(4);
			ft_put_exitcode(envp, 127);
		}
		return (0);
	}
	if (access(path, R_OK | W_OK | X_OK | F_OK) == -1)
		return (ft_print_error(5), ft_put_exitcode(envp, 126), 0);
	if (S_ISREG(path_stat.st_mode))
		return (1);
	else if (S_ISDIR(path_stat.st_mode))
		return (ft_print_error(7), ft_put_exitcode(envp, 126), 0);
	else
		return (ft_print_error(4), ft_put_exitcode(envp, 126), 0);
	return (1);
}
