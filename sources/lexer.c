#include "minishell.h"

char	*extract_variable(char *input, int *len)
{
	char	*start;

	start = input + 1;
	*len = 0;
	input++;
	while (*input && (ft_isalnum(*input) || *input == '_'))
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len));
}

int	quote_nbr(char *input)
{
	int	i;

	i = 0;
	if (input[i] == 39)
	{
		while (input[i] == 39)
			i++;
	}
	else if (input[i] == '"')
	{
		while (input[i] == '"')
			i++;
	}
	if (i % 2 == 0)
		return (1);
	return (0);
}

void	ft_add_all(char **input, t_word **token_list, t_tokens *prev_type)
{
	char	*value;

	if (**input)
	{
		value = ft_strdup(*input);
		add_token(token_list, ARGUMENT, value, 0);
		free(value);
		*prev_type = ARGUMENT;
		(*input)++;
	}
}

int	lexer_options(char *input, t_word **token_list, int pepi,
	t_tokens prev_type)
{
	if (!quote_nbr(input))
		return (ft_add_all(&input, token_list, &prev_type), 0);
	while (*input != '\0')
	{
		while (*input == ' ' || *input == '\t')
			input++;
		if (*input == '|')
			handle_pipe(&input, token_list, &pepi, &prev_type);
		else if (*input == '>' || *input == '<')
			handle_redirection(&input, token_list, &prev_type);
		else if (*input == '\'')
		{
			if (handle_single_quote(&input, token_list, &prev_type))
				return (1);
		}
		else if (*input == '"')
		{
			if (handle_double_quote(&input, token_list, &prev_type))
				return (1);
		}
		else
			if (handle_word(&input, token_list, &prev_type, &pepi))
				return (1);
	}
	return (0);
}

int	lexer(char *input, t_word **token_list)
{
	int			pepi;
	t_tokens	prev_type;

	pepi = 0;
	prev_type = END;
	if(substring(input, ">>>"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
		return (1);
	}
	if(substring(input, "<<<"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		return (1);
	}
	if (lexer_options(input, token_list, pepi, prev_type))
		return (1);
	return (check_quotes_and_finalize(0, 0, token_list));
}
