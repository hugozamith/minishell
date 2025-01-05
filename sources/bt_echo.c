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

int	process_arguments(t_word **current, int fd, int *newline)
{
	if (has_redir(*current))
		*current = rm_redir_node(*current);
	if (fd < 0)
		return (1);
	if (*current && ft_strncmp((*current)->value,
			"-n", ft_strlen((*current)->value)) == 0)
	{
		*newline = 0;
		*current = (*current)->next;
	}
	return (0);
}

int	bt_echo(t_word *args, int fd, char ***envp)
{
	int		fds[2];
	int		newline;
	int		i;
	t_word	*current;

	newline = 1;
	if (handle_exit_code(args, envp) == 0)
		return (0);
	if (init_fds(fds, envp))
		return (1);
	i = handle_redirections(args, envp);
	if (handle_redirection_logic(i, fds, envp) != -1)
		return (1);
	current = args->next;
	if (process_arguments(&current, fd, &newline))
		return (reset_fd(fds[0], fds[1]), ft_put_exitcode(envp, 1), 1);
	ft_put_exitcode(envp, 0);
	print_arguments(current, fd, newline, envp);
	reset_fd(fds[0], fds[1]);
	return (0);
}
