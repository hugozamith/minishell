#include "minishell.h"

void	print_export_sort(char **envp)
{
	int	i;
	int	j;
	char	*temp;

	i = -1;
	while(envp[++i] != NULL)
	{
		j = i;
		while(envp[++j] != NULL)
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
	int 	i;
	int 	j;

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

int	ft_change_found(char ***envp, char *variable, char **argv, int i)
{
	if (has_plus(argv[0]))
	{
		if (!ft_strchr((*envp)[i], '='))
			(*envp)[i] = ft_strjoin_free(ft_strjoin_free((*envp)[i], ft_strdup("=")), ft_strdup(argv[1]));
		else
			(*envp)[i] = ft_strjoin_free((*envp)[i], ft_strdup(argv[1]));
		free(variable);
	}
	else
	{
		free((*envp)[i]);
		(*envp)[i] = variable;
	}
	return (-1);
}

void	ft_create_one(char **envp, char *variable, char **argv, char	*only_arg)
{
	if ((!argv[1] || !ft_strcmp(argv[1], "END")) && !ft_strchr(variable, '=')) 
		*envp = only_arg;
	else if((!argv[1] || !ft_strcmp(argv[1], "END")) && ft_strchr(variable, '='))
		*envp = ft_strjoin_free(only_arg, ft_strdup("="));
	else
		*envp = ft_strjoin_free(ft_strjoin_free(only_arg, ft_strdup("=")), ft_strdup(argv[1]));
	free(variable);
	//free(only_arg);
}

void	update_env(char ***envp, char *variable, char **argv)
{
	int		i;
	char	**argenv;
	char	*only_arg;

	i = -1;
	only_arg = ft_only_arg(argv[0]);
	while ((*envp)[++i])
	{
		argenv = ft_split((*envp)[i], '=');
		if (!ft_strncmp(argenv[0], only_arg, 100))
		{
			i = ft_change_found(envp, variable, argv, i);
			free(only_arg);
			ft_free_argvs(argenv);
			break ;
		}
		ft_free_argvs(argenv);
	}
	if (i != -1)
	{
		*envp = ft_realloc(*envp, (i + 2));
		ft_create_one(&(*envp)[i], variable, argv, only_arg);
		(*envp)[i + 1] = NULL;
	}
	ft_free_argvs(argv);
}




char	*ft_set_variable_value(t_word **dummy, char ***envp)
{
	char	*result;

	if ((*dummy)->value[ft_strlen((*dummy)->value) - 1] == '='
		&& !ft_strchr((*dummy)->next->value, '=')
		&& (*dummy)->value[0] != '=')
	{
		result = ft_strjoin_free(ft_strdup((*dummy)->value),
				expand_string((*dummy)->next, envp));
		*dummy = (*dummy)->next;
		return (result);
	}
	else
		return (expand_string(*dummy, envp));
}

void	ft_bad_checker(char **argv, char *variable, char ***envp)
{
	if (argv)
		ft_free_argvs(argv);
	ft_put_exitcode(envp, 1);
	if (variable)
		free(variable);
}
