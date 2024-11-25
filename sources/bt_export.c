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

int	ft_exportchecker(char **argv, t_shelly **mini)
{
	if (!argv || !argv[0] || !ft_strcmp(argv[0], "1INVALID"))
	{
		(*mini)->exit_code = 1;
		ft_printf_fd(STDERR_FILENO, " not a valid identifier\n");
		return (0);
	}
	if ((ft_strchr(argv[0], '-')))
	{
		(*mini)->exit_code = 1;
		ft_printf_fd(STDERR_FILENO, " not a valid identifier\n");
		return (0);
	}
	if (!(argv[0][0] != '_' && ft_isalpha(argv[0][0])))
	{
		(*mini)->exit_code = 1;
		ft_printf_fd(STDERR_FILENO, " not a valid identifier\n");
		return (0);
	}
	return (1);
}

void ft_put_exitcode(char ***envp, int nbr)
{
	int		i;
	char	*str;

	i = -1;
	str = ft_strjoin("?=", ft_itoa(nbr));
	//ft_printf("VALUE: %s\n", str);
	while ((*envp)[++i])
	{
		if (!ft_strcmp((*envp)[i], "?"))
		{
			free((*envp)[i]);
			(*envp)[i] = str/* ft_strdup(args->next->value) */;
			//ft_printf("FOUND IT!!!");
			ft_printf("Value %s\n", (*envp)[i]);
			return ;
		}
	}
	*envp = (ft_realloc(*envp, (i + 2)));
	(*envp)[i] = str/* ft_strdup(args->next->value) */;
	(*envp)[i + 1] = NULL;
	//ft_printf("Value %s\n", (*envp)[i]);
}

int	bt_export(t_word *args, char ***envp, t_shelly **mini)
{
	int		i;
	char	**argv;

	argv = ft_split(args->next->value, '=');
	*mini = *mini;
	if (!ft_exportchecker(argv, mini))
	{
		//ft_printf("HELLO THERE\n");
		ft_put_exitcode(envp, 1);
		return (0);
	}
	i = -1;
	ft_put_exitcode(envp, 0);
	while ((*envp)[++i])
	{
		if (!ft_strcmp((*envp)[i], argv[0]))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(args->next->value);
			ft_free_argvs(argv);
			return (0);
		}
	}
	ft_free_argvs(argv);
	//ft_printf("VALUE: %s\n", args->next->value);
	*envp = (ft_realloc(*envp, (i + 2)));
	(*envp)[i] = ft_strdup(args->next->value);
	(*envp)[i + 1] = NULL;
	return (0);
}
