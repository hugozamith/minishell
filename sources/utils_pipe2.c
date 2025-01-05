#include "minishell.h"

void	handle_pipe_redirection(int i, int pipe_count, int **pipes)
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
	j = -1;
	while (++j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		free(pipes[j]);
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

void	close_pipes(int pipe_count, int **pipes)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	while (--i >= 0)
		free(pipes[i]);
}

void	ft_guarding_args(t_word *args, int i, int pipe_count)
{
	static t_word	*dummy;

	if (i > 1 && i <= pipe_count)
	{
		if (!dummy)
		{
			ft_printf_fd(0, "NOTHING %d\n", i);
			return ;
		}
		ft_pipe_free(dummy);
	}
	dummy = args;
}

void	ft_free_process_var(t_pipes *pipes_struct)
{
	int	i;

	i = -1;
	if (pipes_struct->command)
		ft_free_args(pipes_struct->command);
	free(pipes_struct->pid);
	free(pipes_struct->retainer);
	free((pipes_struct)->pipes);
	free((pipes_struct));
}
