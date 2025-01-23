#include "minishell.h"

int	handle_pipe(char **input, t_word **token_list, int *pepi,
	t_tokens *prev_type)
{
	add_token(token_list, PIPE, "|", 0);
	(*input)++;
	*pepi = 1;
	*prev_type = PIPE;
	return (0);
}

int	handle_redirection(char **input, t_word **token_list, t_tokens *prev_type)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
			ft_add_redir_token(&input, token_list, REDIRECT_APPEND, ">>");
		else
			ft_add_redir_token(&input, token_list, REDIRECT_OUT, ">");
		*prev_type = REDIRECT_OUT;
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
			ft_add_redir_token(&input, token_list, HEREDOC, "<<");
		else
			ft_add_redir_token(&input, token_list, REDIRECT_IN, "<");
		*prev_type = REDIRECT_IN;
	}
	return (0);
}

int get_flag(char *str, char c)
{
	if (c == 'w')
	{
		while (*str && *str != ' ')
			str++;
		if(*str == ' ')
			return(1);
		return(0);
	}
	while (*(str + 1) && *str != c)
		str++;
	if(*(str + 1) == ' ')
		return(1);
	return(0);
}

int	handle_single_quote(char **input, t_word **token_list, t_tokens *prev_type)
{
	char	*start;
	char	*value;
	int		flag;

	start = (*input)++;
	flag = get_flag((start + 1), '\'');
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\'')
	{
		(*input)++;
		value = ft_strndup(start, *input - start);
		add_token(token_list, ARGUMENT, value, flag);
		free(value);
		*prev_type = ARGUMENT;
		return (0);
	}
	free_tokens(token_list);
	bigproblem();
	return (1);
}

void set_flag_o(t_word **token_list)
{
	t_word	*temp;

	temp = *token_list;
	while (temp->next)
		temp = temp->next;
	temp->_o = 1;
}

int	handle_double_quote(char **input, t_word **token_list, t_tokens *prev_type)
{
	char	*start;
	char	*value;
	int		flag;

	start = (*input)++;
	flag = get_flag((start + 1), '"');
	while (**input && **input != '"') 	
		(*input)++;
	if (**input == '"')
	{
		(*input)++;
		if (**input == '$')
			(*input)++;
		value = ft_strndup(start, *input - start);
		if (!value)
			return (free_tokens(token_list), bigproblem(), 1);
		add_token(token_list, ARGUMENT, value, flag);
		if (**input == '"')
			set_flag_o(token_list);
		free(value);
		*prev_type = ARGUMENT;
		return (0);
	}
	free_tokens(token_list);
	bigproblem();
	return (1);
}

int	handle_word(char **input, t_word **token_list,
	t_tokens *prev_type, int *pepi)
{
	int		len;
	char	*word;

	word = extract_word(*input, &len);
	if (*prev_type == REDIRECT_OUT || *prev_type == REDIRECT_APPEND
		|| *prev_type == REDIRECT_IN || *prev_type == HEREDOC)
	{
		add_token(token_list, ARGUMENT, word, get_flag(*input, 'w'));
	}
	else if (*token_list == NULL || *pepi)
	{
		add_token(token_list, COMMAND, word, get_flag(*input, 'w'));
		*pepi = 0;
	}
	else
	{
		add_token(token_list, ARGUMENT, word,  get_flag(*input, 'w'));
	}
	free(word);
	*input += len;
	*prev_type = ARGUMENT;
	return (0);
}
