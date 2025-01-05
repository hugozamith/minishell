#include "minishell.h"

void	ft_special_node_free(t_word **current)
{
	free((*current)->next->value);
	free((*current)->next);
	free((*current)->value);
	free((*current));
}

void	ft_special_free(t_word *args)
{
	t_word	*old;
	t_word	*dummy;

	dummy = args->prev;
	while (dummy)
	{
		old = dummy->prev;
		free(dummy->value);
		free(dummy);
		dummy = old;
	}
	args->prev = NULL;
}

void	ft_pipe_free(t_word *args)
{
	t_word	*next;

	while (args->type != PIPE)
	{
		next = args->next;
		free(args->value);
		free(args);
		args = next;
	}
}

void	ft_free_split(char ***split)
{
	int	i;

	i = 0;
	while ((*split)[i] != NULL)
	{
		free((*split)[i]);
		(*split)[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*ret;
	char	*a;

	a = (char *)s1;
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ret = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	ft_memcpy(ret, a, len_s1);
	ft_strlcpy(ret + len_s1, s2, len_s2 + 1);
	free(s1);
	free(s2);
	return (ret);
}
