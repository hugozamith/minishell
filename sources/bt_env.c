#include "minishell.h"

void print_env(void)
{
    int i = 0;

    while (g_env[i] != NULL)
    {
        printf("%s\n", g_env[i]);
        i++;
    }
}

int	env_init(void)
{
	extern char	**environ;
	int			i;

	i = split_count(environ);
	if (i == 0)
		return (0);
	g_env = malloc((i + 1) * sizeof(char *));
	if (g_env == NULL)
		return (print_error("minishell", NULL, NULL, strerror(ENOMEM)));
	i = 0;
	while (environ[i])
	{
		g_env[i] = ft_strdup(environ[i]);
		if (g_env[i] == NULL)
		{
			ft_free_split(&g_env);
			return (print_error("minishell", NULL, NULL, strerror(ENOMEM)));
		}
		i++;
	}

	
	g_env[i] = NULL;
	return (0);
}

int bt_env(void)
{
	env_init();
	print_env();

	return(1);
}
