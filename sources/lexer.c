#include "minishell.h"

void add_token(t_word **token_list, t_tokens type, char *value)
{
	t_word *new_token = malloc(sizeof(t_word));
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->next = NULL;

	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		t_word *temp = *token_list;
		while (temp->next) temp = temp->next;
		temp->next = new_token;
	}
}

char *extract_word(char *input, int *len)
{
	char	*start = input;
	*len = 0;
	while (*input && *input != ' ' && *input != '|'
		&& *input != '>' && *input != '<')
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len)); // malloc !!!	
}

/*char *extract_string(char *input, int *len)
{
	char	quote_type = *input;
	*len = 0;
	char *start = input;
	input++;
	(*len) += 2;
	while (*input && *input != quote_type)
	{
		(*len)++;
		input++;
	}
	input++;
	return (ft_strndup(start, *len)); // malloc !!!
}*/

char *extract_variable(char *input, int *len)
{
	char	*start = input + 1;
	*len = 0;
	input++;
	while (*input && (ft_isalnum(*input) || *input == '_'))
	{
		(*len)++;
		input++;
	}
	return (ft_strndup(start, *len)); // malloc !!!
}

void lexer(char *input, t_word **token_list)
{
    int pepi;

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
        else if (*input == '$')
        {
            int len;
            char *var = extract_variable(input, &len);
            add_token(token_list, VARIABLE, var);
            free(var);
            input += len + 1;
        }
        else if (*input == '"' || *input == '\'')
        {
            int len;
            char *str = extract_string(input, &len);
            add_token(token_list, ARGUMENT, str);
            free(str);
            input += len;
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
    add_token(token_list, END, "END");
}
