#include "minishell.h"

void	ft_check_i(int i, char ***envp)
{
	if (i != -1)
		ft_put_exitcode(envp, 0);
}

int	updated_environment(char ***envp, char **argv, t_word *dummy)
{
	int		i;
	char	*variable;

	dummy = dummy->next;
	while (dummy->type == ARGUMENT)
	{
		variable = ft_set_variable_value(&dummy, envp);
		argv = ft_split(variable, '=');
		i = -1;
		if (!ft_exportchecker(argv))
		{
			ft_bad_checker(argv, variable, envp);
			if (dummy->next->type == END)
				break ;
			else
				i = -2 ;
		}
		if (i != -2)
			update_env(envp, variable, argv);
		if (!dummy->next)
			break ;
		dummy = dummy->next;
	}
	ft_check_i(i, envp);
	return (0);
}

int	bt_export(t_word *args, char ***envp)
{
	char	**argv;
	t_word	*dummy;
	int		status;

	argv = NULL;
	dummy = args;
	status = handle_export_input(dummy, envp);
	if (!status)
	{
		return (status);
	}
	status = updated_environment(envp, argv, dummy);
	return (status);
}
