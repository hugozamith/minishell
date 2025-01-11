#include "minishell.h"

int	bt_pwd(t_word *args)
{
	char	cwd[1024];

	if (!ft_strncmp(args->next->value, "-", 1))
		return (ft_print_error(10), 0);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
	}
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}
