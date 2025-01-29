#include "minishell.h"

int	ft_is_many_arguments(t_word *args, char ***envp)
{
	char	*line;
	char	**arguments;
	char	*old_arg;
	int		exit_number;

	expand_args(args, envp);
	line = ft_args_to_line(args);
	arguments = ft_split(line, ' ');
	free(line);
	if ((!ft_strcmp(arguments[1], "-")
			|| !ft_strcmp(arguments[1], "+")) && arguments[2])
	{
		old_arg = arguments[1];
		arguments[1] = ft_strjoin(arguments[1], arguments[2]);
		free(old_arg);
		free(arguments[2]);
		arguments[2] = NULL;
	}
	if (arguments[2])
		return (ft_free_argvs(arguments), 0);
	if (!atoi(arguments[1]))
		return (ft_free_argvs(arguments), 1);
	exit_number = atoi(arguments[1]);
	ft_free_argvs(arguments);
	return (exit_number);
}

int	handle_exit_no_args_or_invalid(t_word *args, char ***envp)
{
	char	*arg;
	int		exit_code;

	if (args->next->type == END
		|| ft_strlen(args->next->value) <= 0)
	{
		printf("exit\n");
		arg = ft_getenv("?", envp);
		if (!arg)
		{
			ft_free_all(envp, &args);
			exit(g_code_of_exit);
		}
		exit_code = ft_atoi(arg);
		ft_free_all(envp, &args);
		g_code_of_exit = exit_code;
		exit(g_code_of_exit);
	}
	if (!ft_is_many_arguments(args, envp))
	{
		ft_print_error(1);
		ft_put_exitcode(envp, 1);
		return (0);
	}
	return (-1);
}

void	validate_and_exit_with_arg(t_word *args, char ***envp, int exit_status)
{
	char	*arg;

	arg = args->next->value;
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg != '\0')
	{
		if (!ft_isdigit(*arg))
		{
			if (ft_isalpha(*arg))
			{
				ft_print_error(2);
				ft_free_all(envp, &args);
				exit(2);
			}
		}
		arg++;
	}
	printf("exit\n");
	ft_free_all(envp, &args);
	exit(exit_status);
}

int	bt_exit(t_word *args, char ***envp)
{
	int	exit_status;

	exit_status = handle_exit_no_args_or_invalid(args, envp);
	if (exit_status != -1)
		return (exit_status);
	exit_status = ft_is_many_arguments(args, envp);
	validate_and_exit_with_arg(args, envp, exit_status);
	return (0);
}
