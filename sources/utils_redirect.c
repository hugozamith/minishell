#include "minishell.h"

int	ft_is_bt(char *word)
{
	if (!ft_strncmp(word, "echo", 5))
		return (1);
	if (!ft_strncmp(word, "cd", 3))
		return (1);
	if (!ft_strncmp(word, "pwd", 4))
		return (1);
	if (!ft_strncmp(word, "export", 7))
		return (1);
	if (!ft_strncmp(word, "unset", 6))
		return (1);
	if (!ft_strncmp(word, "env", 4))
		return (1);
	return (0);
}

int	is_bts_in_pipe(t_word *args)
{
	int		i;
	int		j;
	int		pipe;
	int		redirect;
	char	*str;

	i = 0;
	j = 0;
	pipe = 0;
	redirect = 0;
	str = args->value;
	while ((args || (pipe == 0 && redirect == 0)) && args->type != PIPE)
	{
		if (args->type == PIPE)
		{
			pipe = i;
		}
		if (args->type == REDIRECT_APPEND || args->type == REDIRECT_IN
			|| args->type == REDIRECT_OUT)
		{
			if (redirect)
				redirect = -1;
			else
				redirect = j;
		}
		i++;
		j++;
		args = args->next;
	}
	pipe = i;
	if (!ft_is_bt(str))
		return (0);
	else if (redirect < pipe && pipe && redirect && redirect != -1)
	{
		ft_print_error(-1);
		return (1);
	}
	return (0);
}

char	*merge_filename(t_word *node)
{
	char	*new_value;
	int		i;

	new_value = ft_strdup("");
	while (node->type == ARGUMENT)
	{
		i = 0;
		if (node->value[i] == '"')
			i++;
		while (node->value[i] != '"' && node->value[i])
		{
			new_value = add_char(new_value, node->value[i]);
			i++;
		}
		if (node->O == 1)
		{
			node = node->next;
		}
		else
			break ;
	}
	return (new_value);
}

int	count_nodes(t_word *head)
{
	int		count;
	t_word	*current;

	count = 0;
	current = head;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	reset_fd(int saved_fd_in, int saved_fd_out)
{
	if (dup2(saved_fd_in, STDIN_FILENO) == -1)
	{
		perror("reset_fd: dup2(STDIN_FILENO) failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(saved_fd_out, STDOUT_FILENO) == -1)
	{
		perror("reset_fd: dup2(STDOUT_FILENO) failed");
		exit(EXIT_FAILURE);
	}
}
