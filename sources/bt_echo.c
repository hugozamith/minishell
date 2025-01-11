#include "minishell.h"

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
