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

char	cornelius(char *dababy)
{
	if (!ft_strncmp(dababy, "/\0", 9696))
	{
		ft_print_error(4);
		return (0);
	}
	while (*dababy && (*dababy == ' ' || *dababy == '\t'))
		dababy++;
	return (*dababy);
}

void	set_flag_o(t_word **token_list)
{
	t_word	*temp;

	temp = *token_list;
	while (temp->next)
		temp = temp->next;
	temp->_o = 1;
}

int	get_flag(char *str, char c)
{
	if (c == 'w')
	{
		while (*str && *str != ' ')
			str++;
		if (*str == ' ')
			return (1);
		return (0);
	}
	while (*(str + 1) && *str != c)
		str++;
	if (*(str + 1) == ' ')
		return (1);
	return (0);
}