#include "minishell.h"

void	ft_put_in_my_env(char ***envp, char *cwd)
{
	int		i;
	char	**old_pwd;

	i = -1;
	while((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], "PWD=", 4))
		{
			old_pwd = ft_split((*envp)[i], '=');
			free((*envp)[i]);
			(*envp)[i] = ft_strjoin("PWD=", cwd);
		}
	}
	i = -1;
	while((*envp)[++i])
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

	if (!args->next) // No argument after "cd"
	{
		path = getenv("HOME");
		if (!path)
		{
			perror("cd: HOME not set");
			return (1);
		}
	}
	else
	{
		args = args->next;
		path = expand_string(args);
		//printf("path: %s\n", path);
	}
	//ft_printf("Value %s", args->next->next->value);
	if (ft_strcmp(args->next->next->value, "END"))
	{
		ft_put_exitcode(envp, 1);
		ft_printf_fd(STDERR_FILENO, " too many arguments\n");
		return (0);
	}
	if (!ft_strcmp(path, "$PWD"))
		return (0);
	if (chdir(path) != 0) // Change directory
	{
		ft_put_exitcode(envp, 1);
		perror("cd");
		//ft_printf_fd(STDERR_FILENO, " not a valid identifier\n");
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL) // Print new directory
	{
		ft_put_in_my_env(envp, cwd);
		//printf("Directory changed to: %s\n", cwd);
	}
	else
	{
		perror("getcwd");
		return (1);
	}
	return (0);
}
