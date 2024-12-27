#include "minishell.h"

int	ft_is_many_arguments(t_word *args, char ***envp)
{
	char	*line;
	char	**arguments;
	char	*old_arg;
	int		exit_number;

	expand_args(args, envp);
	line = ft_args_to_line(args);
	//ft_printf("FIRST\n");
	//printf("Value: %s\n", line);
	arguments = ft_split(line, ' ');
	free(line);
	/* printf("VALUE: %s\n", arguments[0]);
	printf("VALUE: %s\n", arguments[1]);
	printf("VALUE: %s\n", arguments[2]);
	ft_printf("SECOND\n"); */
	if ((!ft_strcmp(arguments[1], "-") || !ft_strcmp(arguments[1], "+")) && arguments[2])
	{
		//printf("BEFORE: %s\n", arguments[1]);
		old_arg = arguments[1];
		arguments[1] = ft_strjoin(arguments[1], arguments[2]);
		free(old_arg);
		//printf("AFTER: %s\n", arguments[1]);
		free(arguments[2]);
		arguments[2] = NULL;
	}
	//ft_printf("THIRD\n");
	if (arguments[2])
	{
		ft_free_argvs(arguments);
		return (0);
	}
	if (!atoi(arguments[1]))
	{
		ft_free_argvs(arguments);
		return (1);
	}
	exit_number = atoi(arguments[1]);
	ft_free_argvs(arguments);
	return (exit_number);
}

int	bt_exit(t_word *args, char ***envp)
{
	int		exit_status;
	char	*arg;
	int		exit_code;

	//printf("exit\n");
	//printf("Value: %d\n", (*mini)->exit_code);
	if (!ft_strncmp(args->next->value, "END", 3))
	{
		printf("exit\n");
		//ft_printf("FIRST\n");
		arg = ft_getenv("?", envp);
		if (!arg)
			exit(0);
		//ft_printf("SECOND\nVALUE: %s\n", arg);
		exit_code = ft_atoi(arg);
		//ft_printf("THIRD\n");
		ft_free_all(envp, &args);
		exit(exit_code);
	}
	exit_status = ft_is_many_arguments(args, envp);
	if (!exit_status)
	{
		//ft_printf_fd(STDERR_FILENO, " too many arguments\n");
		ft_print_error(1);
		//ft_free_all(envp, &args);
		ft_put_exitcode(envp, 1);
		return (0);
		//exit (1);
	}
	//printf("Value: %s\n", args->next->next->value);
	arg = args->next->value;
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
		{
			arg = expand_string(args->next, envp);
			//printf("Value: %s\n", arg);
			if(!ft_atoi(arg))
			{
				//ft_printf_fd(STDERR_FILENO, " numeric argument required\n");
				ft_print_error(2);
				ft_free_all(envp, &args);
				free(arg); 
				exit(2);
			}
			free(arg);
		}
		arg++;
	}
	//exit_status = atoi(args->next->value);
	//ft_printf("Value: %d\n", exit_status);
	printf("exit\n");
	ft_free_all(envp, &args);
	exit(exit_status);
}
