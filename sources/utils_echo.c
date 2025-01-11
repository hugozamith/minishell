#include "minishell.h"

int	tokensrch(t_word *args, t_tokens token)
{
	while (args)
	{
		if (args->type == token)
			return (1);
		args = args->next;
	}
	return (0);
}

int	ft_just_exit_code(t_word *args)
{
	if (!ft_strcmp(args->value, "echo")
		&& !ft_strncmp(args->next->value, "$?", 3)
		&& !ft_strcmp(args->next->next->value, "END"))
		return (1);
	return (0);
}

t_word	*remove_single_redirection_node(t_word *current)
{
	t_word	*prev;

	if (current->prev->type == COMMAND)
	{
		while (current->next->_o == 1)
			current = current->next;
		prev = current->prev;
		current->prev->next = current->next->next;
		ft_special_node_free(&current);
		return (prev->next);
	}
	while (current->next->_o == 1)
		current = current->next;
	prev = current->prev;
	current->prev->next = current->next->next;
	ft_special_node_free(&current);
	return (prev);
}

t_word	*rm_redir_node(t_word *args)
{
	t_word	*current;
	t_word	*next;

	current = args;
	while (current)
	{
		next = current->next;
		if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND
			|| current->type == HEREDOC)
		{
			return (remove_single_redirection_node(current));
		}
		current = next;
	}
	return (args);
}

int	has_redir(t_word *args)
{
	while (args)
	{
		if (args->type == REDIRECT_IN || args->type == REDIRECT_OUT
			|| args->type == REDIRECT_APPEND || args->type == HEREDOC)
		{
			if (args->type == REDIRECT_OUT)
				return (2);
			else
				return (1);
		}
		args = args->next;
	}
	return (0);
}
