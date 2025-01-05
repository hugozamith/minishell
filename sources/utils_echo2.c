#include "minishell.h"

/* int	bt_echo(t_word *args, int fd, char ***envp)
{
	int		newline;
	t_word	*current;
	char	*expanded;
	int		fds[2];
	int		i;
	int		redir;

	i = 0;
	if (ft_just_exit_code(args))
	{
		expanded = expand_string(args->next, envp);
		if (*expanded == '1' || *expanded == '2')
			return (ft_printf("%s\n", expanded), free(expanded), 0);
		free(expanded);
	}
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
		return (perror("dup failed"), ft_put_exitcode(envp, 1), 1);
	newline = 1;
	i = handle_redirections(args, envp);
	if (i < 0)
		return (ft_put_exitcode(envp, 1), reset_fd(fds[0], fds[1]), 1);
	if (i == 69)
		return (ft_put_exitcode(envp, 2), reset_fd(fds[0], fds[1]),
			ft_print_error(6), 0);
	current = args->next;
	redir = has_redir(current);
	if (redir)
		current = rm_redir_node(current);
	if (fd < 0)
		return (reset_fd(fds[0], fds[1]), ft_put_exitcode(envp, 1), 1);
	if (current && ft_strncmp(current->value, "-n",
			ft_strlen(current->value)) == 0)
	{
		newline = 0;
		current = current->next;
	}
	ft_put_exitcode(envp, 0);
	while (current && current->type == ARGUMENT)
	{
		expanded = expand_string(current, envp);
		ft_putstr_fd(expanded, fd);
		free(expanded);
		if (current->next && current->next->type != END
			&& current->next->type == ARGUMENT
			&& !(current->next->value[0] == '\''
				|| current->next->value[0] == '"')
			&& !(current->value[0] == '\'' || current->value[0] == '"'))
			ft_putchar_fd(' ', fd);
		current = current->next;
	}
	if (newline)
		ft_putchar_fd('\n', fd);
	if (fd != STDOUT_FILENO)
		close(fd);
	reset_fd(fds[0], fds[1]);
	if (redir == 3)
		ft_redirect_free(current, envp);
	return (0);
} */

int	handle_exit_code(t_word *args, char ***envp)
{
	char	*expanded;

	if (ft_just_exit_code(args))
	{
		expanded = expand_string(args->next, envp);
		if (*expanded == '1' || *expanded == '2')
		{
			ft_printf("%s\n", expanded);
			free(expanded);
			return (0);
		}
		free(expanded);
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

	while (current && current->type == ARGUMENT)
	{
		expanded = expand_string(current, envp);
		ft_putstr_fd(expanded, fd);
		free(expanded);
		if (current->next && current->next->type == ARGUMENT
			&& ft_strncmp(current->next->value, "'", 1)
			&& !ft_empty(current))
			ft_putchar_fd(' ', fd);
		current = current->next;
	}
	if (newline)
		ft_putchar_fd('\n', fd);
}
