#include "minishell.h"

int	bt_echo(t_word *args, int fd)
{
	int	newline;
	t_word	*current;

	newline = 1;
	current = args->next;

	fd = handle_redirections(args);
	remove_quotations(&current->value);
	if (fd < 0)
		return (1);
	if (current &&
		ft_strncmp(current->value, "-n", (ft_strlen(current->value) + 5)) == 0)
	{
		newline = 0;
		current = current->next;
	}
	while (current && current->type == ARGUMENT)
	{
		ft_putstr_fd(current->value, fd);
		if (current->next)
			ft_putchar_fd(' ', fd);
		current = current->next;
	}
	if (newline)
		ft_putchar_fd('\n', fd);
	if (fd != STDOUT_FILENO)
		close(fd);
	return (0);
}
