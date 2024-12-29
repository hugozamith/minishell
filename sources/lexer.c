#include "minishell.h"

void	bigproblem(void)
{
	ft_putstr_fd("unclosed quotes!!!!! naughty naughty!!\n",
		STDERR_FILENO);
}

void	free_tokens(t_word **token_list)
{
	t_word	*current;
	t_word	*next;

	current = *token_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*token_list = NULL;
}

void	add_token(t_word **token_list, t_tokens type, char *value, int flag)
{
	t_word	*new_token;
	t_word	*temp;

	new_token = malloc(sizeof(t_word));
	if (!new_token)
		exit(EXIT_FAILURE);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	new_token->prev = NULL;
	if (flag == 1)
		new_token->O = 1;
	else
		new_token->O = 0;
	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		temp = *token_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

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

int	lexer(char *input, t_word **token_list)
{
	int			flag;
	int			pepi;
	t_tokens	prev_type;
	int			single_quote_open;
	int			double_quote_open;
	char		*start;
	int			len;
	char		*word;
	char		*value;

	prev_type = END;
	single_quote_open = 0;
	double_quote_open = 0;
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
			prev_type = PIPE;
		}
		else if (*input == '>')
		{
			if (*(input + 1) == '>')
			{
				add_token(token_list, REDIRECT_APPEND, ">>", 0);
				input += 2;
			}
			else
			{
				add_token(token_list, REDIRECT_OUT, ">", 0);
				input++;
			}
			prev_type = REDIRECT_OUT;
		}
		else if (*input == '<')
		{
			if (*(input + 1) == '<')
			{
				add_token(token_list, HEREDOC, "<<", 0);
				input += 2;
			}
			else
			{
				add_token(token_list, REDIRECT_IN, "<", 0);
				input++;
			}
			prev_type = REDIRECT_IN;
		}
		else if (*input == '\'')
		{
			start = input++;
			while (*input && *input != '\'')
				input++;
			if (*input == '\'')
			{
				input++;
				value = ft_strndup(start, input - start);
				add_token(token_list, ARGUMENT, value, 0);
				free(value);
			}
			else
			{
				free_tokens(token_list);
				bigproblem();
				return (1);
			}
			prev_type = ARGUMENT;
		}
		else if (*input == '"')
		{
			start = input++;
			while (*input && *input != '"')
				input++;
			if (*input == '"')
			{
				input++;
				if (*input == '$')
					input++;
				if (*input == '"')
				{
					flag = 1;
				}
				value = ft_strndup(start, input - start);
				add_token(token_list, ARGUMENT, value, flag);
				free(value);
			}
			else
			{
				free_tokens(token_list);
				bigproblem();
				return (1);
			}
			prev_type = ARGUMENT;
		}
		else
		{
			word = extract_word(input, &len);
			if (prev_type == REDIRECT_OUT || prev_type == REDIRECT_APPEND
				|| prev_type == REDIRECT_IN || prev_type == HEREDOC)
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
			prev_type = ARGUMENT;
		}
	}
	if (single_quote_open || double_quote_open)
	{
		free_tokens(token_list);
		bigproblem();
		return (1);
	}
	add_token(token_list, END, "END", 0);
	return (0);
}
