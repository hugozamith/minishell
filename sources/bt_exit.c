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

int	bt_exit(t_word *args, char ***envp)
{
	int		exit_status;
	char	*arg;
	int		exit_code;

	if (!ft_strncmp(args->next->value, "END", 3))
	{
		printf("exit\n");
		arg = ft_getenv("?", envp);
		if (!arg)
			exit(0);
		exit_code = ft_atoi(arg);
		ft_free_all(envp, &args);
		exit(exit_code);
	}
	exit_status = ft_is_many_arguments(args, envp);
	if (!exit_status)
	{
		ft_print_error(1);
		ft_put_exitcode(envp, 1);
		return (0);
	}
	arg = args->next->value;
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
		{
			arg = expand_string(args->next, envp);
			if (!ft_atoi(arg))
			{
				ft_print_error(2);
				ft_free_all(envp, &args);
				free(arg);
				exit(2);
			}
			free(arg);
		}
		arg++;
	}
	printf("exit\n");
	ft_free_all(envp, &args);
	exit(exit_status);
}
