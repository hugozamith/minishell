#include "minishell.h"

int	ft_env_size(char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
		i++;
	return (i);
}

int	ft_arg_not_exist(t_word *args, char ***envp)
{
	int		i;
	char	**word;

	i = -1;
	while ((*envp)[++i])
	{
		word = ft_split((*envp)[i], '=');
		if (!ft_strncmp(word[0], args->next->value, 100))
			return (ft_free_argvs(word), 0);
		ft_free_argvs(word);
	}
	return (1);
}

int	bt_unset(t_word *args, char ***envp)
{
	int		i;
	int		j;
	char	**new_envp;
	char	**word;

	if (!ft_strncmp(args->next->value, "-", 1))
		return (ft_print_error(10), 0);
	i = ft_env_size(envp);
	if (ft_arg_not_exist(args, envp))
		return (0);
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		return (perror("malloc"), 1);
	i = -1;
	j = -1;
	while ((*envp)[++i])
	{
		word = ft_split((*envp)[i], '=');
		if (ft_strncmp(word[0], args->next->value, 100))
			new_envp[++j] = ft_strdup((*envp)[i]);
		ft_free_argvs(word);
		free((*envp)[i]);
	}
	return (new_envp[++j] = NULL, free(*envp), *envp = new_envp, 0);
}
