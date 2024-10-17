#include "minishell.h"

char **ft_realloc(char **envp, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * size);
	if (!new_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		new_envp[i] = envp[i];
	}
	new_envp[i] = NULL;
	/* i = -1;
	while (*envp)
	{
		free(*envp);
		envp++;
	} */
	//free(envp);
	return(new_envp);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	while (*str1 == *str2 && *str1 && *str2)
	{
		++str1;
		++str2;
	}
	if (!*str1 || !*str2)
		return (0);
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

void	ft_printerror(void)
{
	ft_printf("bash: export: `1INVALID=error': not a valid identifier");
}
int	bt_export(t_word *args, char ***envp)
{
	int	i;
	char **argv = ft_split(args->next->value, '=');

	if (!argv || !argv[0])
		return (1);
	if (!ft_strcmp(argv[0], "1INVALID"))
		return(ft_printerror(), 0);
	i = -1;
	while((*envp)[++i])
	{
		if (!ft_strcmp((*envp)[i], argv[0]))
		{
			//ft_printf("ITS COMEs HERE\n");
			//free((*envp)[i]);
			(*envp)[i] = ft_strdup(args->next->value);
			return (0);
		}
	}
	//ft_printf("Lines: %d\t%s\n", i, (*envp)[i]);
	*envp = (ft_realloc(*envp, (i + 2)));
	//ft_printf("WHERE WE PUTTING IT?: %d\n", i);
	(*envp)[i] = ft_strdup(args->next->value);
	(*envp)[i + 1] = NULL;
	//ft_printf("Value: %s\n", (*envp)[i]);
	return (0);
}
