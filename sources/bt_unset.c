#include "minishell.h"

int	bt_unset(t_word *args, char ***envp)
{
	int		i;
	int		j;
	char	**new_envp;
	char	**word;

	i = 0;
	j = 0;
	while ((*envp)[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while ((*envp)[++i])
	{
		word = ft_split((*envp)[i], '=');
		if (ft_strcmp(word[0], args->next->value))
		{
			new_envp[j] = (*envp)[i];
			j++;
		}
		else
			free((*envp)[i]);
		ft_free_argvs(word);
	}
	new_envp[j] = NULL;
	return (free(*envp), *envp = new_envp, 0);
}
