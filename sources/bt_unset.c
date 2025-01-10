#include "minishell.h"

int	bt_unset(t_word *args, char ***envp)
{
	int		i;
	int		j;
	char	**new_envp;
	char	**word;

	if (!ft_strncmp(args->next->value, "-", 1))
		return (ft_print_error(10), 0);
	i = 0;
	while ((*envp)[i])
		i++;
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
