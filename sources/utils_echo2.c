#include "minishell.h"

int	handle_exit_code(t_word *args, char ***envp)
{
	(void) envp;
	if (ft_just_exit_code(args))
	{
		ft_printf("%d\n", g_code_of_exit);
		return (0);
	}
	return (-1);
}

int	init_fds(int *fds, char ***envp)
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
	{
		perror("dup failed");
		ft_put_exitcode(envp, 1);
		return (1);
	}
	return (0);
}

int	handle_redirection_logic(int i, int *fds, char ***envp)
{
	if (i < 0)
	{
		ft_put_exitcode(envp, 1);
		reset_fd(fds[0], fds[1]);
		return (1);
	}
	if (i == 69)
	{
		ft_put_exitcode(envp, 2);
		reset_fd(fds[0], fds[1]);
		ft_print_error(6);
		return (0);
	}
	return (-1);
}

int	ft_empty(t_word *current)
{
	if (ft_strlen(current->value) == 2 && (*current->value == 34)
		&& (*(current->value + 1) == 34))
		return (1);
	if (ft_strlen(current->next->value) == 2 && (*current->next->value == 34)
		&& (*(current->next->value + 1) == 34))
		return (1);
	return (0);
}

void	print_arguments(t_word *current, int fd, int newline, char ***envp)
{
	char	*expanded;
	char	*tmp;

	while (current && current->type == ARGUMENT)
	{
		expanded = expand_string(current, envp);
		tmp = expanded;
		if (*expanded == '-')
		{
			tmp = expanded;
			tmp++;
			while (*tmp == 'n')
				tmp++;
			if (!(*tmp))
				newline = 0;
		}
		if (*tmp)
			ft_putstr_fd(expanded, fd);
		if (current->next && current->next->type == ARGUMENT
			&& current->_a == 1	&& *tmp && !ft_empty(current))
			ft_putchar_fd(' ', fd);
		current = current->next;
		free(expanded);
	}
	if (newline)
		ft_putchar_fd('\n', fd);
}
