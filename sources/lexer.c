#include "minishell.h"

char	*extract_word(char *input, int *len)
{
	char	*start;

	start = input;
	*len = 0;
	while (*input && *input != ' ' && *input != '|'
		&& *input != '>' && *input != '<' && *input != '"' && *input != '\'')
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len));
}

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

int	lexer_options(char *input, t_word **token_list, int pepi,
	t_tokens prev_type)
{
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
	if (lexer_options(input, token_list, pepi, prev_type))
		return (1);
	return (check_quotes_and_finalize(0, 0, token_list));
}
