#include "minishell.h"

void	handle_pipe_redirection(int i, int pipe_count, int pipes[][2])
{
	int	j;

	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 error");
			exit(EXIT_FAILURE);
		}
	}
	if (i < pipe_count)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error");
			exit(EXIT_FAILURE);
		}
	}
	j = 0;
	while (j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

char	*command_to_str(t_word *command)
{
	char	*str;
	t_word	*temp;

	str = ft_strdup(command->value);
	temp = command->next;
	while (temp)
	{
		str = ft_strjoin_free(str, ft_strdup(" "));
		str = ft_strjoin_free(str, temp->value);
		temp = temp->next;
	}
	return (str);
}

void	close_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	ft_special_free(t_word *args)
{
	t_word	*old;
	t_word	*dummy;

	dummy = args->prev;
	while (dummy)
	{
		old = dummy->prev;
		free(dummy->value);
		free(dummy);
		dummy = old;
	}
	args->prev = NULL;
}

void	ft_pipe_free(t_word *args)
{
	t_word	*next;

	while (args->type != PIPE)
	{
		next = args->next;
		free(args->value);
		free(args);
		args = next;
	}
}
