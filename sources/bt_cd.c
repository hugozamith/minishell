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

int	bt_cd(t_word *args, char ***envp)
{
	char	*path;
	char	cwd[1024];

	if (!args->next)
	{
		path = ft_getenv("HOME", envp);
		if (!path)
		{
			perror("cd: HOME not set");
			return (1);
		}
	}
	else
	{
		args = args->next;
		path = expand_string(args, envp);
	}
	if (ft_strcmp(args->next->value, "END"))
	{
		free(path);
		ft_put_exitcode(envp, 1);
		ft_print_error(1);
		return (0);
	}
	if (!ft_strcmp(path, "$PWD"))
	{
		free(path);
		return (0);
	}
	if (chdir(path) != 0)
	{
		free(path);
		ft_put_exitcode(envp, 1);
		perror("cd");
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_put_in_my_env(envp, cwd);
	}
	else
	{
		free(path);
		perror("getcwd");
		return (1);
	}
	free(path);
	return (0);
}
