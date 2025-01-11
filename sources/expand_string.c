#include "minishell.h"

void	situation_bar(char **current, char **result)
{
	char	*expanded;

	(*current)++;
	expanded = handle_single_quotes(*current);
	*result = ft_strjoin_free(*result, expanded);
	while (**current && **current != '\'')
		(*current)++;
}

int	other_situations(char **current, char **result, char ***envp)
{
	char	*expanded;

	if (**current == '"')
	{
		(*current)++;
		expanded = handle_double_quotes(*current, envp);
		*result = ft_strjoin_free(*result, expanded);
		while (**current && **current != '"')
			(*current)++;
		if (**current == '"')
			(*current)++;
		if (**current == '$')
			return (*result = add_char(*result, **current), 1);
		return (0);
	}
	else if (**current == '\'')
		situation_bar(current, result);
	else if (**current != '\'' && **current != '"'
		&& **current != '$')
	{
		*result = add_char(*result, **current);
		(*current)++;
	}
	return (0);
}

char	*expand_string(t_word *input, char ***envp)
{
	char	*result;
	char	*current;
	char	*expanded;

	result = ft_strdup("");
	current = input->value;
	while (*current)
	{
		if (*current == '$' && *(current + 1) != ' ')
		{
			if (*(current + 1) == '\0')
				return (handle_shenanigans(result, NULL));
			expanded = expand_variable(current++, envp);
			if (*current++ != '?')
				while (*current && (ft_isalnum(*current)
						|| *current == '_' || *current == '?'))
					current++;
			result = ft_strjoin_free(result, expanded);
		}
		else
			if (other_situations(&current, &result, envp))
				return (result);
	}
	return (result);
}