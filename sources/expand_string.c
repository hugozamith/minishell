#include "minishell.h"

// Function to expand variables like `$VAR`
char	*expand_variable(char *str)
{
	char	*var_name;
	char	*value;
	char	*var;

	// Skip the '$' character
	str++;
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
		if (*str == '$')  // Expand variables inside double quotes
		{
			expanded = expand_variable(str);
			result = ft_strjoin_free(result, expanded);
			str += ft_strlen(expanded);  // Move past the expanded variable
			return (result);
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
			if (*current == '\'')
				current++;  // Skip closing single quote
		}
		else if (*current == '$')  // Handle variable expansion
		{
			expanded = expand_variable(current);
			result = ft_strjoin_free(result, expanded);
			current += (ft_strlen(expanded) - 3);  // Move the pointer past the expanded variable
			if (current < input->value)
				current = input->value;
			//printf("Current after bling: %s\n", current);
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
