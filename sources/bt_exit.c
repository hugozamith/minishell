#include "minishell.h"

int	bt_exit(t_word *args, char ***envp)
{
	int		exit_status;
	char	*arg;

	//printf("exit\n");
	if (!ft_strncmp(args->next->value, "END", 3))
	{
		printf("exit\n");
		ft_free_all(envp, &args);
		exit(EXIT_SUCCESS);
	}
	arg = args->next->value;
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
		{
			printf("exit: %s: numeric argument required\n", args->next->value);
			exit(2);
		}
		arg++;
	}
	exit_status = atoi(args->next->value);
	printf("exit\n");
	ft_free_all(envp, &args);
	exit(exit_status);
}
