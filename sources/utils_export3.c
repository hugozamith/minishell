#include "minishell.h"

void	print_export_sort(char **envp)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (envp[++i] != NULL)
	{
		j = i;
		while (envp[++j] != NULL)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
		}
	}
}

int	handle_export_input(t_word *args, char ***envp)
{
	if (!ft_strncmp(args->value, "export", 6)
		&& !ft_strncmp(args->next->value, "END", 3))
	{
		print_export_sort(*envp);
		print_export(*envp);
		return (0);
	}
	return (1);
}

char	*ft_only_arg(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (has_plus(str))
	{
		result = malloc(sizeof(char) * (ft_strlen(str)));
		while (str[++i])
		{
			if (str[i] != '+')
			{
				result[j] = str[i];
				j++;
			}
		}
		result[j] = '\0';
	}
	else
		result = ft_strdup(str);
	return (result);
}
