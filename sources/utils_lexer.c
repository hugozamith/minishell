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

int	handle_single_quote(char **input, t_word **token_list, t_tokens *prev_type)
{
	char	*start;
	char	*value;

	start = (*input)++;
	while (**input && **input != '\'')
		(*input)++;
	if (**input == '\'')
	{
		(*input)++;
		value = ft_strndup(start, *input - start);
		add_token(token_list, ARGUMENT, value, 0);
		free(value);
		*prev_type = ARGUMENT;
		return (0);
	}
	free_tokens(token_list);
	bigproblem();
	return (1);
}

int	handle_double_quote(char **input, t_word **token_list, t_tokens *prev_type)
{
	char	*start;
	char	*value;
	int		flag;

	start = (*input)++;
	flag = 0;
	while (**input && **input != '"')
		(*input)++;
	if (**input == '"')
	{
		(*input)++;
		if (**input == '$')
			(*input)++;
		if (**input == '"')
			flag = 1;
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

int	handle_word(char **input, t_word **token_list,
	t_tokens *prev_type, int *pepi)
{
	int		len;
	char	*word;

	word = extract_word(*input, &len);
	if (*prev_type == REDIRECT_OUT || *prev_type == REDIRECT_APPEND
		|| *prev_type == REDIRECT_IN || *prev_type == HEREDOC)
	{
		add_token(token_list, ARGUMENT, word, 0);
	}
	else if (*token_list == NULL || *pepi)
	{
		add_token(token_list, COMMAND, word, 0);
		*pepi = 0;
	}
	else
	{
		add_token(token_list, ARGUMENT, word, 0);
	}
	free(word);
	*input += len;
	*prev_type = ARGUMENT;
	return (0);
}
