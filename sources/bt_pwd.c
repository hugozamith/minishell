#include "minishell.h"

int	bt_pwd(void)
{
	char	cwd[1024];

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
