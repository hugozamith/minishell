#include "minishell.h"

/* char	*ft_getenv(char *var, char ***envp)
{
	int		i;
	char	**str;
	char	*result;

	i = -1;
	while ((*envp)[++i])
	{
		str = ft_split((*envp)[i], '=');
		if (!ft_strcmp(str[0], var))
		{
			if (str[1])
				result = ft_strdup(str[1]);
			else
				result = NULL;
			ft_free_argvs(str);
			return (result);
		}
		ft_free_argvs(str);
	}
	return (NULL);
}

char	*handle_shenanigans(char *result, char *current)
{
	char	*expanded;

	if (current)
	{
		if (*(current + 1) == '?')
		{
			free(result);
			return (ft_strdup("0\0"));
		}
	}
	expanded = ft_strdup("$\0");
	free(result);
	return (expanded);
}

char	*expand_variable(char *str, char ***envp)
{
	char	*var_name;
	char	*value;
	char	*var;

	str++;
	if (*str == '?')
	{
		value = ft_getenv(str, envp);
	}
	else
	{
		var_name = str;
		while (*str && (ft_isalnum(*str) || *str == '_'))
			str++;
		var = ft_strndup(var_name, str - var_name);
		value = ft_getenv(var, envp);
		free(var);
	}
	if (!value)
		return (ft_strdup(""));
	return (value);
}

char	*handle_double_quotes(char *str, char ***envp)
{
	char	*expanded;
	char	*result;

	result = ft_strdup("");
	while (*str && *str != '"')
	{
		if (*str == '$' && *(str + 1) != ' ')
		{
			if (*(str + 2) == '\0')
				return (handle_shenanigans(result, NULL));
			expanded = expand_variable(str++, envp);
			if (*str++ != '?')
				while (*str && (ft_isalnum(*str) || *str == '_' || *str == '?'))
					str++;
			result = ft_strjoin_free(result, expanded);
		}
		else
		{
			result = add_char(result, *str);
			str++;
		}
	}
	return (result);
}

char	*handle_single_quotes(char *str)
{
	char	*result;

	result = ft_strdup("");
	while (*str && *str != '\'')
	{
		result = add_char(result, *str);
		str++;
	}
	return (result);
} */

/* char	*something_else(char **current, char *result,
	char ***envp, char *expanded)
{
	if (**current == '\'')
	{
		(*current)++;
		expanded = handle_single_quotes(*current);
		result = ft_strjoin_free(result, expanded);
		while (**current && **current != '\'')
			(*current)++;
	}
	else if (**current == '$' && **(current + 1) != ' ')
	{
		if (**(current + 1) == '\0')
			return (handle_shenanigans(result, NULL));
		expanded = expand_variable((*current)++, envp);
		if (**current++ != '?')
			while (**current && (ft_isalnum(**current)
					|| **current == '_' || **current == '?'))
				(*current)++;
		result = ft_strjoin_free(result, expanded);
	}
	else if (**current != '\'' && **current != '"'
		&& **current != '$')
	{
		result = add_char(result, **current);
		(*current)++;
	}
	return (NULL);
} */

/* char *something_else(char **current, char **result,
	char ***envp, char *expanded)
{
	if (**current == '\'')
	{
		(*current)++;
		expanded = handle_single_quotes(*current);
		*result = ft_strjoin_free(*result, expanded);
		while (**current && **current != '\'')
			(*current)++;
		if (**current == '\'')
			(*current)++;
	}
	else if (**current == '$' && *(*current + 1) != ' ')
	{
		ft_printf("VALUE: %c\n", **current);
		if (*(*current + 1) == '\0')
			return (handle_shenanigans(*result, NULL));
		expanded = expand_variable((*current)++, envp);
		if (**current != '?')
			while (**current && (ft_isalnum(**current) 
				|| **current == '_' || **current == '?'))
				(*current)++;
		*result = ft_strjoin_free(*result, expanded);
	}
	else if (**current != '\'' && **current != '"' && **current != '$')
	{
		*result = add_char(*result, **current);
		(*current)++;
	}
	return (NULL);
} */

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

/* int something_else3(char **current, char **result, char ***envp)
{
	char	*expanded;
	if (**(current + 1) == '\0')
	{
		*result = handle_shenanigans(*result, NULL);
		return (1);
	}
	expanded = expand_variable((*current)++, envp);
	if (**current++ != '?')
		while (**current && (ft_isalnum(**current)
				|| **current == '_' || **current == '?'))
			(*current)++;
	*result = ft_strjoin_free(*result, expanded);
	return (0);
} */

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

/* else if (*current == '\'')
		{
			current++;
			expanded = handle_single_quotes(current);
			result = ft_strjoin_free(result, expanded);
			while (*current && *current != '\'')
				current++;
		}
		else if (*current == '$' && *(current + 1) != ' ')
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
		else if (*current != '\'' && *current != '"'
			&& *current != '$')
		{
			result = add_char(result, *current);
			current++;
		} */