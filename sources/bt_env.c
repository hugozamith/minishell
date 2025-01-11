#include "minishell.h"

char	***ft_special_env(void)
{
	char	***str;
	char	cwd[1024];

	str = malloc(sizeof(char **) * 1);
	str[0] = malloc(sizeof(char *) * (3 + 1));
	if (!str)
		return (NULL);
	str[0][0] = ft_strjoin_free(ft_strdup("PWD="),
			ft_strdup(getcwd(cwd, sizeof(cwd))));
	str[0][1] = ft_strdup("SHLVL=1");
	str[0][2] = ft_strdup("_=/usr/bin/env");
	str[0][3] = NULL;
	return (str);
}

void	print_specific_env(t_word *args, char **envp)
{
	int		i;
	char	**str;
	char	*path;

	i = -1;
	path = args->next->value;
	while (envp[++i] != NULL)
	{
		str = ft_split(envp[i], '=');
		if (!ft_strncmp(str[0], (path + 1), ft_strlen(path)))
		{
			ft_free_argvs(str);
			printf("%s\n", envp[i]);
			return ;
		}
		ft_free_argvs(str);
	}
	print_env(envp);
}

void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	i = -1;
	while (envp[++i] != NULL)
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
	}
}

char	***env_init(char **envp)
{
	int		i;
	char	***str;

	i = 0;
	if (!envp || !envp[0])
		return (ft_special_env());
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
		if (!str[0][i])
		{
			while (i > 0)
				free(str[0][--i]);
			return (free(str[0]), free(str), NULL);
		}
	}
	str[0][i] = NULL;
	return (str);
}

int	bt_env(t_word *args, char **envp)
{
	if (!ft_strncmp(args->next->value, "$", 1))
		print_specific_env(args, envp);
	else if (!ft_strncmp(args->next->value, "END", 4)
		|| !ft_strncmp(args->next->value, "|", 2))
		print_env(envp);
	else
		ft_print_error(9);
	return (0);
}
