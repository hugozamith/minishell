#include "minishell.h"

void	ft_free_argvs(char **argv)
{
	int	i;

	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv);
}

void	ft_free_env(char ***env)
{
	int	i;

	i = -1;
	if (env[0] && *env[0])
	{
		while (env[0][++i])
		{
			free(env[0][i]);
		}
		free(*env);
	}
	free(env);
	env = NULL;
}

void	ft_free_args(t_word *args)
{
	t_word	*old_args;

	while (args->next)
	{
		old_args = args;
		args = (args)->next;
		free(old_args->value);
		free(old_args);
	}
	free(args->value);
	free(args);
	args = NULL;
}

void	ft_free_all(char ***env, t_word **args)
{
	if (env && *env)
	{
		ft_free_env(env);
	}
	if (args && *args)
	{
		ft_free_args(*args);
	}
}

void	ft_redirect_free(t_word *current, char ***env)
{
	t_word	*dummy;
	t_word	*current_dummy;
	t_word	*next;
	int		i;

	dummy = current;
	i = 0;
	ft_put_exitcode(env, 5);
	current_dummy = current;
	while (current_dummy->prev)
	{
		if (current_dummy->type == REDIRECT_OUT
			|| current_dummy->type == REDIRECT_APPEND
			|| current_dummy->type == REDIRECT_IN)
			dummy = current_dummy;
		current_dummy = current_dummy->prev;
	}
	while (i <= 1)
	{
		next = dummy->next;
		free(dummy->value);
		free(dummy);
		dummy = next;
		i++;
	}
}
