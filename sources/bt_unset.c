#include "minishell.h"


int bt_unset(t_word *args, char ***envp)
{
	int	i;
	int	j;
	char **new_envp;
	char **word;

	i = 0;
	j = 0;
	while ((*envp)[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	i = 0;
	//ft_printf("Env: %s\n", args->next->value);
	while((*envp)[i])
	{
		word = ft_split((*envp)[i], '=');
		if (ft_strcmp(word[0], args->next->value))
		{
			new_envp[j] = (*envp)[i];
			j++;
		}
		/* else
			ft_printf("FOUND IT!\n"); */
		i++;
		//ft_printf("Env: %s\n", envp[i]);
	}
	if (!new_envp[j])
		free(new_envp[j+1]);
	//ft_printf("Env: %s\n", new_envp[j- 1]);
	*envp = new_envp; 
	return (0);
}
