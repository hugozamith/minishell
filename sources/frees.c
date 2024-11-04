#include "minishell.h"

void	ft_free_argvs(char **argv)
{
	int	i;

	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv);
}
void	ft_free_env(char **env)
{
	int	i;

	i = -1;
	if(env && env[0])
	{
		//ft_printf("HELLO\n");
		while (env[++i])
		{
			free(env[i]);
		}
		//ft_printf("After: %s\n", env[i]);
		free(env);
		env = NULL;
	}
	//ft_printf("After: %d\n", i);
}
void	ft_free_args(t_word **args)
{
	int	i;

	i = -1;
	while(args[++i])
		free(args[i]);
	//free(args);
}

void	ft_free_all(char **env, t_word **args)
{
	if (env)
		ft_free_env(env);
	if (args)
		ft_free_args(args);
}