#include "minishell.h"

char	*ft_getenv(char *var, char ***envp)
{
	int		i;
	char	**str;
	char	*result;

	i = -1;
	//ft_printf("VALUE: %s\n", var);
	while ((*envp)[++i])
	{
		//ft_printf("ENV VALUE: %s\n", (*envp)[i]);
		str = ft_split((*envp)[i], '=');
		if (!ft_strcmp(str[0], var))
		{
			/* ft_printf("ENV VALUE: %s\n", var);
			ft_printf("ENV VALUE: %s\n", (*envp)[i]); */
			result = ft_strdup(str[1]);
			/* free(str[0]);
			free(str[1]);
			free(str); */
			ft_free_argvs(str);
			//ft_free_env(envp);
			return (result);
		}
		ft_free_argvs(str);
		/* free(str[0]);
		free(str[1]);
		free(str); */
	}
	return (NULL);
}

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
char	*expand_variable(char *str, char ***envp)
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
		value = ft_getenv(str, envp);
	}
	else
	{
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
		value = ft_getenv(var, envp);
		//ft_getenv(var);
		free(var);  // Free the variable name string
	}
	if (!value)
		return (ft_strdup(""));  // If variable not found, return empty string
	return (value);  // Return the value as a new string
}

// Handle double-quoted string
char	*handle_double_quotes(char *str, char ***envp)
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
			expanded = expand_variable(str++, envp);
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
char	*expand_string(t_word *input, char ***envp)
{
	char	*result;
	char	*current;
	char	*expanded;

	result = ft_strdup("");  // Initialize result string
	current = input->value;
	while (*current)
	{
		if (*current == '"')  // Handle double quotes
		{
			//printf("Current: %s\n", current);
			current++;
			expanded = handle_double_quotes(current, envp);
			result = ft_strjoin_free(result, expanded);
			while (*current && *current != '"')
				current++;
			//printf("Current: %s\n", current);
			if (*current == '"')
				current++;  // Skip closing double quote
			//printf("Current: %s\n", current);
			if(*current == '$')
			{
			//	printf("result: %s\n", result);
				result = add_char(result, *current);
				return (result);
			}
				//return (add_char(result, *current));
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
			expanded = expand_variable(current++, envp);
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
