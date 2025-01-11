#include "minishell.h"

int	handle_export_input(t_word *args, char ***envp)
{
	if (!ft_strncmp(args->value, "export", 6)
		&& !ft_strncmp(args->next->value, "END", 3))
	{
		print_export(*envp);
		return (0);
	}
	return (1);
}

void	update_env(char ***envp, char *variable, char **argv)
{
	int		i;
	char	**argenv;

	i = -1;
	while ((*envp)[++i])
	{
		argenv = ft_split((*envp)[i], '=');
		if (!ft_strncmp(argenv[0], argv[0], 100))
		{
			free((*envp)[i]);
			(*envp)[i] = variable;
			ft_free_argvs(argenv);
			i = -1;
			break ;
		}
		ft_free_argvs(argenv);
	}
	if (i != -1)
	{
		*envp = ft_realloc(*envp, (i + 2));
		(*envp)[i] = variable;
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
