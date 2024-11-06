#include "minishell.h"

void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	i = -1;
	while (envp[++i] != NULL)
	{
		printf("%s\n", envp[i]);
	}
}

char	***env_init(char **envp)
{
	int		i;
	char	***str;

	i = 0;
	str = malloc(sizeof(char **) * 1);
	while (envp[i])
		i++;
	str[0] = malloc(sizeof(char *) * (i + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		str[0][i] = ft_strdup(envp[i]);
		//ft_printf("DOING SOMETHING\n");
		if (!str[0][i])  // Check for allocation failure
		{
			while (i > 0)
			{
				free(str[0][--i]);
			}
			free(str[0]);
			free(str);
			return (NULL);
		}
	}
	str[0][i] = NULL;
	return (str);
}

int	bt_env(char **envp)
{
	print_env(envp);
	return (0);
}
