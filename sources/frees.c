#include "minishell.h"

void    ft_free_argvs(char **argv)
{
    int i;
    i = -1;
    while (argv[++i])
        free(argv[i]);
    free(argv);
}
void	ft_free_env(char ***env)
{
	int	i;

	i = -1;
	//ft_printf("FREEING ENV\n");
	if(env[0] && *env[0])
	{
		//ft_printf("HELLO\n");
		//ft_printf("FIRST\n");
		while (env[0][++i])
		{
			free(env[0][i]);
		}
		//ft_printf("SECOND\n");
		//ft_printf("After: %s\n", env[i]);
		free(*env);
	}
	free(env);
	env = NULL;
	//ft_printf("After: %d\n", i);
}
void    ft_free_args(t_word *args)
{
	t_word *old_args;

	//ft_printf("THIRD\n");
	//ft_printf("FREEING ARGS\n");
	/* if (args->prev)
		while (args->prev)
			args = args->prev; */
	while(args->next)
	{
		//ft_printf("VALUE: %s\n", old_args->value);
		old_args = args;
		args = (args)->next;
		free(old_args->value);
		free(old_args);
		/* if (args == NULL)
			break ; */
	}
	//ft_printf("FOURTH\n");
	free(args->value);
	free(args);
	args = NULL;
	/* if (args)
		free(args); */
}

void	ft_free_all(char ***env, t_word **args)
{
	//ft_printf("VALUE: %s\n", (*args)->value);
	//ft_printf("VALUE: %s\n", args[1]->value);
	//ft_printf("FIRST\n");
	//ft_printf_fd(0, "Got in\n");
	if (env && *env)
		ft_free_env(env);
	//ft_printf("SECOND\n");
	if (args && *args)
	{
		//ft_printf_fd(0, "HEREEEE\n");
	 	ft_free_args(*args);
	}
	/* else if (!args)
		ft_printf("HEERE\n");
	else if (!*args)
		ft_printf("HEERE1\n"); */
}