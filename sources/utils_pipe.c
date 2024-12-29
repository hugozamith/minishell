#include "minishell.h"

void	delete_token_list(t_word **token_list)
{
	t_word	*current;
	t_word	*next_token;

	current = *token_list;
	while (current)
	{
		next_token = current->next;
		free(current->value);
		free(current);
		current = next_token;
	}
	*token_list = NULL;
}

char	*add_char(char *str, char c)
{
	char	*new_str;
	int		len;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	ft_memcpy(new_str, str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

int	count_pipes(t_word *args)
{
	int	count;

	count = 0;
	while (args)
	{
		if (args->type == PIPE)
			count++;
		args = args->next;
	}
	return (count);
}

t_word	*get_next_command(t_word **args)
{
	t_word	*command;
	t_word	*pipe_token;

	command = *args;
	while (*args && (*args)->type != PIPE)
	{
		*args = (*args)->next;
	}
	if (*args && (*args)->type == PIPE)
	{
		pipe_token = *args;
		*args = (*args)->next;
		pipe_token->next = NULL;
	}
	return (command);
}

void	create_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
