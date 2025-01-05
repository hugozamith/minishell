#include "minishell.h"

char	**ft_realloc(char **envp, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * size);
	if (!new_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		free(envp[i]);
	}
	free(envp);
	new_envp[i] = NULL;
	return (new_envp);
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

int	ft_exportchecker(char **argv)
{
	if (!argv || !argv[0] || !ft_strcmp(argv[0], "1INVALID"))
	{
		ft_print_error(3);
		return (0);
	}
	if ((ft_strchr(argv[0], '-')))
	{
		ft_print_error(3);
		return (0);
	}
	if (!(argv[0][0] != '_' && ft_isalpha(argv[0][0])))
	{
		ft_print_error(3);
		return (0);
	}
	return (1);
}

void	ft_put_exitcode(char ***envp, int nbr)
{
	int		i;
	char	*str;
	char	*number;

	i = -1;
	number = ft_itoa(nbr);
	str = ft_strjoin("?=", number);
	free(number);
	while ((*envp)[++i])
	{
		if (!ft_strcmp((*envp)[i], "?"))
		{
			free((*envp)[i]);
			(*envp)[i] = str;
			return ;
		}
	}
	*envp = (ft_realloc(*envp, (i + 2)));
	(*envp)[i] = str;
	(*envp)[i + 1] = NULL;
}
