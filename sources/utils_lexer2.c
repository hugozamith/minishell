#include "minishell.h"

void	bigproblem(void)
{
	ft_putstr_fd("unclosed quotes!!!!! naughty naughty!!\n",
		STDERR_FILENO);
}

void	free_tokens(t_word **token_list)
{
	t_word	*current;
	t_word	*next;

	current = *token_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*token_list = NULL;
}

void	add_token(t_word **token_list, t_tokens type, char *value, int flag)
{
	t_word	*new_token;
	t_word	*temp;

	new_token = malloc(sizeof(t_word));
	if (!new_token)
		exit(EXIT_FAILURE);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	new_token->prev = NULL;
	if (flag == 1)
		new_token->_o = 1;
	else
		new_token->_o = 0;
	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		temp = *token_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

void	ft_add_redir_token(char ***input, t_word **token_list,
	t_tokens type, char *value)
{
	add_token(token_list, type, value, 0);
	if (type == REDIRECT_APPEND || type == HEREDOC)
		**input += 2;
	else
		(**input)++;
}

int	check_quotes_and_finalize(int single_quote_open,
	int double_quote_open, t_word **token_list)
{
	if (single_quote_open || double_quote_open)
	{
		free_tokens(token_list);
		bigproblem();
		return (1);
	}
	add_token(token_list, END, "END", 0);
	return (0);
}
