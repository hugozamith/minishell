#include "minishell.h"

int	split_count(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
		i++;
	return (i);
}

int	print_error(char *s1, char *s2, char *s3, char *message)
{
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
	{
		if (s1)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(s2, 2);
	}
	if (s3)
	{
		if (s1 || s2)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	if (message)
	{
		if (s1 || s2 || s3)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(message, 2);
	}
	ft_putchar_fd('\n', 2);
	return (-1);
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

char	*ft_strndup(const char *s, int n)
{
	size_t	size;
	char	*str;
	char	*ret;

	size = ft_strlen(s) + 1;
	if ((int)size > n)
		size = n + 1;
	str = malloc(size);
	if (NULL == str)
		return (NULL);
	ret = str;
	while (*s && n--)
		*str++ = *s++;
	*str = '\0';
	return (ret);
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
