#include "minishell.h"

void	ft_special_node_free(t_word **current)
{
	free((*current)->next->value);
	free((*current)->next);
	free((*current)->value);
	free((*current));
}
