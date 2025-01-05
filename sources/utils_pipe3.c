#include "minishell.h"

void	ft_malloc_pipe(t_pipes **pipes_struct)
{
	int	i;

	i = -1;
	while (++i < (*pipes_struct)->pipe_count)
	{
		(*pipes_struct)->pipes[i] = malloc(sizeof(int) * 2);
		if (!(*pipes_struct)->pipes[i])
		{
			while (--i >= 0)
				free((*pipes_struct)->pipes[i]);
			free((*pipes_struct)->pipes);
			free(*pipes_struct);
			*pipes_struct = NULL;
			return ;
		}
	}
}

void	ft_init_pipes_struct(t_pipes **pipes_struct, t_word *args)
{
	*pipes_struct = malloc(sizeof(t_pipes));
	if (!(*pipes_struct))
		return ;
	(*pipes_struct)->pipe_count = count_pipes(args);
	(*pipes_struct)->pipes = malloc(sizeof(int *)
			* (*pipes_struct)->pipe_count);
	ft_malloc_pipe(pipes_struct);
	(*pipes_struct)->status = 0;
	create_pipes((*pipes_struct)->pipe_count, (*pipes_struct)->pipes);
	(*pipes_struct)->pid = malloc(sizeof(int)
			* ((*pipes_struct)->pipe_count + 1));
	(*pipes_struct)->i = 0;
	(*pipes_struct)->retainer = malloc(sizeof(t_word)
			* ((*pipes_struct)->pipe_count + 1));
	(*pipes_struct)->retainer[0] = NULL;
	(*pipes_struct)->retainer[(*pipes_struct)->pipe_count] = NULL;
}
