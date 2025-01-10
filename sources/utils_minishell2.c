#include "minishell.h"

int	ft_extras(char **word, t_word **args, char ***envp)
{
	if (!ft_strncmp((*args)->value, "$PWD", 4))
	{
		ft_printf_fd(STDERR_FILENO, " Is a directory\n");
		return (ft_put_exitcode(envp, 126), 0);
	}
	else if (!ft_strncmp((*args)->value, "$", 2)
		|| !ft_strncmp((*args)->value, "$?", 3))
	{
		ft_printf_fd(STDERR_FILENO, " command not found\n");
		return (ft_put_exitcode(envp, 127), 0);
	}
	else if (!ft_strncmp((*args)->value, "$EMPTY", 6))
	{
		if (ft_strncmp((*args)->next->value, "END", 3))
			return (*args = (*args)->next,
				*word = (*args)->value, 1);
		return (ft_put_exitcode(envp, 0), 0);
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
		return (bt_pwd(args));
	if (!ft_strncmp(word, "export", 7))
		return (bt_export(args, envp));
	if (!ft_strncmp(word, "unset", 6))
		return (bt_unset(args, envp));
	if (!ft_strncmp(word, "env", 4))
		return (bt_env(args, *envp));
	if (!ft_strncmp(word, "exit", 5))
		return (bt_exit(args, envp));
	return (1);
}

void	ft_free_line_arguments(t_word **args)
{
	t_word	*next;

	while ((*args))
	{
		next = (*args)->next;
		free((*args)->value);
		free((*args));
		(*args) = next;
	}
}
