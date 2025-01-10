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

void	print_export(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	i = -1;
	while (envp[++i] != NULL)
	{
		printf("declare -x %s\n", envp[i]);
	}
}

void	expand_args(t_word *args, char ***envp)
{
	t_word	*temp;
	char	*str;

	temp = args;
	if (args->next->type == END && ft_strchr(args->value, ' '))
		return ;
	while (temp)
	{
		if (temp->type == ARGUMENT)
		{
			str = expand_string(temp, envp);
			free(temp->value);
			temp->value = str;
		}
		temp = temp->next;
	}
}
