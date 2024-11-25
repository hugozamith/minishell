#include "minishell.h"

void bigproblem()
{
    ft_putstr_fd("unclosed quotes!!!!! naughty naughty!!\n", STDERR_FILENO);
}

void free_tokens(t_word **token_list)
{
	t_word	*current;
	t_word	*next;

	current = *token_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);  // Free the token value
		free(current);         // Free the token itself
		current = next;
	}
	*token_list = NULL; // Set the token list pointer to NULL
	}

void add_token(t_word **token_list, t_tokens type, char *value)
{
	t_word *new_token = malloc(sizeof(t_word));
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	new_token->prev = NULL;  // Initialize prev to NULL

	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		t_word *temp = *token_list;
		while (temp->next) temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;  // Set the previous pointer to the last token
	}
}

char *extract_word(char *input, int *len)
{
	char *start = input;
	*len = 0;
	while (*input && *input != ' ' && *input != '|'
		&& *input != '>' && *input != '<' && *input != '"' && *input != '\'')
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len)); // malloc !!!
}

char *extract_variable(char *input, int *len)
{
	char *start = input + 1;
	*len = 0;
	input++;
	while (*input && (ft_isalnum(*input) || *input == '_'))
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len)); // malloc !!!
}
int lexer(char *input, t_word **token_list)
{
	int pepi;
	int single_quote_open = 0;
	int double_quote_open = 0;

	while (*input != '\0')
	{
		while (*input == ' ' || *input == '\t')
			input++;
		
		if (*input == '|')
		{
			add_token(token_list, PIPE, "|");
			input++;
			pepi = 1;
		}
		else if (*input == '>')
		{
			if (*(input + 1) == '>')
			{
				add_token(token_list, REDIRECT_OUT, ">>");
				input++;
			}
			else
				add_token(token_list, REDIRECT_OUT, ">");
			input++;
		}
		else if (*input == '<')
		{
			if (*(input + 1) == '<')
			{
				add_token(token_list, REDIRECT_IN, "<<");
				input++;
			}
			else
				add_token(token_list, REDIRECT_IN, "<");
			input++;
		}
		else if (*input == '\'')
		{
			char *start = input++; // Start of single-quoted string
			while (*input && *input != '\'') input++; // Skip until closing single quote
			
			if (*input == '\'') {
				input++;  // Skip the closing quote
				char *value = ft_strndup(start, input - start);
				add_token(token_list, ARGUMENT, value); // Include quotes
				free(value);
			} else {
				free_tokens(token_list);  // Free tokens if no closing quote
				bigproblem();
				return (1);
			}
		}
		else if (*input == '"')
		{
			char *start = input++; // Start of double-quoted string
			while (*input && *input != '"') input++; // Skip until closing double quote
			if (*input == '"')
			{

				input++;
				if  (*input == '$')
					input++;
				char *value = ft_strndup(start, input - start);
//					if (*(input + 1) == '$')
//					value = add_char(value, *(input++ + 1));
				add_token(token_list, ARGUMENT, value); // Include quotes
				free(value);
			} else {
				free_tokens(token_list);  // Free tokens if no closing quote
				bigproblem();
				return (1);
			}
		}
		else
		{
			int len;
			char *word = extract_word(input, &len);
			if (*token_list == NULL || pepi)
			{
				add_token(token_list, COMMAND, word);
				pepi = 0;
			}
			else
				add_token(token_list, ARGUMENT, word);
			free(word);
			input += len;
		}
	}
	// Check if quotes are unclosed
	if (single_quote_open || double_quote_open)
	{
		free_tokens(token_list);  // Free the tokens if quotes are unclosed
		bigproblem();             // Call bigproblem if quotes are unclosed
		return (1);               // Return 1 to indicate an error
	}

	add_token(token_list, END, "END");
	return (0); // Return 0 for success
}
