#include "minishell.h"

int	handle_redirections(t_word *args)
{
	t_word	*current;
	int	fd;

	current = args;

	while (current)
	{
		if (current->type == REDIRECT_OUT)
		{
			fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				return (-1);
			}
			return (fd);
		}
		else if (current->type == REDIRECT_IN)
		{
			fd = open(current->next->value, O_RDONLY);
			if (fd < 0)
			{
				perror("open");
				return (-1);
			}
			return (fd);
		}
		current = current->next;
	}
	return (STDOUT_FILENO);
}
