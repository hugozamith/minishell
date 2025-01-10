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
