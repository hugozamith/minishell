#include "minishell.h"

char *handle_shenanigans(char *result, char *current)
{
	char *expanded;

	//write(1, "m", 1);
	if (current)
		if (*(current + 1) == '?')
		{
			free(result);
			return (ft_strdup("0\0"));
		}
	expanded = ft_strdup("$\0");
	free(result);
	return (expanded);
}

// Function to expand variables like `$VAR`
char	*expand_variable(char *str)
{
	char	*var_name;
	char	*value;
	char	*var;
	// Skip the '$' character
	str++;
	// zif (*str == '$')
	// {
	// 	str++;
	// 	return (ft_strdup("937709979"));
	// }

	if (*str == '?')
	{
		str++;
		return (ft_itoa(0));//          to do!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	// if (*str == '?')
	// {
	// 	str++;
	// 	return (ft_itoa(0));//          to do!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// }
	// Extract the variable name
	var_name = str;
	while (*str && (ft_isalnum(*str) || *str == '_'))
		str++;
	// Create a null-terminated variable name string
	var = ft_strndup(var_name, str - var_name);
	// Use getenv or custom function to fetch the variable value
	value = getenv(var);
	free(var);  // Free the variable name string
	if (!value)
		return (ft_strdup(""));  // If variable not found, return empty string
	return (ft_strdup(value));  // Return the value as a new string
}

// Handle double-quoted string
char	*handle_double_quotes(char *str)
{
	char	*expanded;
	char	*result;

	result = ft_strdup("");  // Initialize result string
	while (*str && *str != '"')
	{
		if (*str == '$' && *(str + 1) != ' ')  // Expand variables inside double quotes
		{
			if (*(str + 2) == '\0')
				return (handle_shenanigans(result, NULL));
			expanded = expand_variable(str++);
			//printf ("Expanded: -%s-       -%zu\n", expanded, ft_strlen(expanded));
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

// Handle single-quoted string (no expansion)
char	*handle_single_quotes(char *str)
{
	char	*result;

	result = ft_strdup("");  // Initialize result string
	while (*str && *str != '\'')
	{
		result = add_char(result, *str);
		str++;
	}
	return (result);  // Return literal content inside single quotes
}

// Expander function
char	*expand_string(t_word *input)
{
	char	*result;
	char	*current;
	char	*expanded;

	result = ft_strdup("");  // Initialize result string
	current = input->value;
	while (*current)
	{
		//printf("Current: %s\n", current);
		if (*current == '"')  // Handle double quotes
		{
			current++;
			expanded = handle_double_quotes(current);
			result = ft_strjoin_free(result, expanded);
			while (*current && *current != '"')
				current++;
			if (*current == '"')
				current++;  // Skip closing double quote
		}
		else if (*current == '\'')  // Handle single quotes (no expansion)
		{
			current++;
			expanded = handle_single_quotes(current);
			result = ft_strjoin_free(result, expanded);
			while (*current && *current != '\'')
				current++;
			//if (*current == '\'')
			//	current++;  // Skip closing single quote
		}
		else if (*current == '$' && *(current + 1) != ' ')  // Expand variables inside double quotes
		{
			if (*(current + 1) == '\0')
				return (handle_shenanigans(result, NULL));
			expanded = expand_variable(current++);
			//printf ("Expanded: -%s-       -%zu\n", expanded, ft_strlen(expanded));
			if (*current++ != '?')
				while (*current && (ft_isalnum(*current) || *current == '_' || *current == '?'))
					current++;
			result = ft_strjoin_free(result, expanded);
		}
		else if (*current != '\'' && *current != '"'
			&& *current != '$') // Normal character handling
		{
			result = add_char(result, *current);
			current++;
		}
	}
	return (result);
}
