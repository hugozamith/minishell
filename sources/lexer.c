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

void add_token(t_word **token_list, t_tokens type, char *value, int flag)
{
	t_word *new_token = malloc(sizeof(t_word));
	if (!new_token)
		exit(EXIT_FAILURE); // Garantir que o malloc foi bem-sucedido
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	new_token->prev = NULL;  // Initialize prev to NULL=
	if (flag == 1)
	{
		//write(1, "aqui2\n", 6);
		new_token->O = 1;
	}
	else
		new_token->O = 0;
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
    int flag;
    int pepi;
    t_tokens prev_type = END; // Track the previous token type
    int single_quote_open = 0;
    int double_quote_open = 0;

    while (*input != '\0')
    {
        flag = 0;
        while (*input == ' ' || *input == '\t')
            input++;

        if (*input == '|')
        {
            add_token(token_list, PIPE, "|", 0);
            input++;
            pepi = 1;
            prev_type = PIPE; // Update previous token type
        }
        else if (*input == '>')
        {
            if (*(input + 1) == '>')
            {
                add_token(token_list, REDIRECT_APPEND, ">>", 0);
                input += 2; // Skip ">>"
            }
            else
            {
                add_token(token_list, REDIRECT_OUT, ">", 0);
                input++;
            }
            prev_type = REDIRECT_OUT; // Update previous token type
        }
        else if (*input == '<')
        {
            if (*(input + 1) == '<')
            {
                add_token(token_list, HEREDOC, "<<", 0);
                input += 2; // Skip "<<"
            }
            else
            {
                add_token(token_list, REDIRECT_IN, "<", 0);
                input++;
            }
            prev_type = REDIRECT_IN; // Update previous token type
        }
        else if (*input == '\'')
        {
            char *start = input++; // Start of single-quoted string
            while (*input && *input != '\'') input++; // Skip until closing single quote

            if (*input == '\'')
            {
                input++; // Skip the closing quote
                char *value = ft_strndup(start, input - start);
                add_token(token_list, ARGUMENT, value, 0); // Include quotes
                free(value);
            }
            else
            {
                free_tokens(token_list); // Free tokens if no closing quote
                bigproblem();
                return (1);
            }
            prev_type = ARGUMENT; // Update previous token type
        }
        else if (*input == '"')
        {
            char *start = input++; // Start of double-quoted string
            while (*input && *input != '"') input++; // Skip until closing double quote
            if (*input == '"')
            {
                input++;
                if (*input == '$')
                    input++;
                if (*input == '"')
                {
                    flag = 1;
                }

                char *value = ft_strndup(start, input - start);
                add_token(token_list, ARGUMENT, value, flag); // Include quotes
                free(value);
            }
            else
            {
                free_tokens(token_list); // Free tokens if no closing quote
                bigproblem();
                return (1);
            }
            prev_type = ARGUMENT; // Update previous token type
        }
        else
        {
            int len;
            char *word = extract_word(input, &len);

            if (prev_type == REDIRECT_OUT || prev_type == REDIRECT_APPEND || 
                prev_type == REDIRECT_IN || prev_type == HEREDOC)
            {
                add_token(token_list, ARGUMENT, word, 0);
            }
            else if (*token_list == NULL || pepi)
            {
                add_token(token_list, COMMAND, word, 0);
                pepi = 0;
            }
            else
            {
                add_token(token_list, ARGUMENT, word, 0);
            }

            free(word);
            input += len;
            prev_type = ARGUMENT; // Update previous token type
        }
    }

    // Check if quotes are unclosed
    if (single_quote_open || double_quote_open)
    {
        free_tokens(token_list); // Free the tokens if quotes are unclosed
        bigproblem(); // Call bigproblem if quotes are unclosed
        return (1); // Return 1 to indicate an error
    }

    add_token(token_list, END, "END", 0);
    return (0); // Return 0 for success
}
