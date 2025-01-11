#include "minishell.h"

void	ft_put_in_my_env(char ***envp, char *cwd)
{
	int		i;
	char	**old_pwd;

	i = -1;
	while ((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], "PWD=", 4))
		{
			old_pwd = ft_split((*envp)[i], '=');
			free((*envp)[i]);
			(*envp)[i] = ft_strjoin("PWD=", cwd);
		}
	}
	i = -1;
	while ((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], "OLDPWD=", 7))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strjoin("OLDPWD=", old_pwd[1]);
		}
	}
	ft_free_argvs(old_pwd);
}

int	handle_home_directory(char ***envp, char ***path)
{
	**path = ft_getenv("HOME", envp);
	if (!**path)
	{
		perror("cd: HOME not set");
		return (1);
	}
	return (0);
}
