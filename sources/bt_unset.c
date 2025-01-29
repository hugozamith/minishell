#include "minishell.h"

int ft_env_size(char ***envp)
{
	int i;

	i = 0;
	while ((*envp)[i])
		i++;
	return (i);
}

int ft_arg_not_exist(t_word *args, char ***envp)
{
	int i;
	char **word;

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

int ft_start_unset(t_word *args, char ***envp, char **word, int size)
{
	int i;
	int j;
	char **new_envp;

	new_envp = malloc(sizeof(char *) * size);
	if (!new_envp)
		return (perror("malloc"), 1);
	i = -1;
	j = -1;
	if (ft_arg_not_exist(args, envp))
		return (0);
	while ((*envp)[++i])
	{
		word = ft_split((*envp)[i], '=');
		if (ft_strncmp(word[0], args->next->value, 100))
			new_envp[++j] = ft_strdup((*envp)[i]);
		ft_free_argvs(word);
		free((*envp)[i]);
	}
	new_envp[++j] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

int ft_unset_prepare(t_word *current, char ***envp, char **word, int size)
{
	if (!ft_strncmp(current->next->value, "-", 1))
		return (ft_print_error(10), 0);
	size = ft_env_size(envp);
	if (ft_arg_not_exist(current, envp))
		return (0);
	return (ft_start_unset(current, envp, word, size), 0);
}

int bt_unset(t_word *args, char ***envp)
{
	int size;
	char **word;
	t_word *current;

	current = args;
	word = NULL;
	size = 0;
	while (current->next->type == ARGUMENT)
	{
		ft_unset_prepare(current, envp, word, size);
		current = current->next;
	}
	return (0);
}
