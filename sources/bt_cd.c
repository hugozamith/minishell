#include "minishell.h"

int get_cd_path(t_word *args, char ***envp, char **path)
{
	//ft_printf("FIRST\n");
	if (args->next->type == END)
		return (handle_home_directory(envp, &path));
	args = args->next;
	*path = expand_string(args, envp);
	return (0);
}

int check_end_token(t_word *args, char **path, char ***envp)
{
	//ft_printf("SECOND\n");
	if (!args->next->next)
		return (0);
	if (ft_strcmp(args->next->next->value, "END"))
	{
		free(*path);
		*path = NULL;
		ft_put_exitcode(envp, 1);
		ft_print_error(1);
		return (1);
	}
	return (0);
}

int change_directory(char **path, char ***envp)
{
	//ft_printf("THIRD\n");
	if (!ft_strcmp(*path, "$PWD"))
	{
		free(*path);
		*path = NULL;
		return (0);
	}
	if (chdir(*path) != 0)
	{
		free(*path);
		*path = NULL;
		ft_put_exitcode(envp, 1);
		perror("cd");
		return (0);
	}
	return (0);
}

int update_environment(char ***envp, char **path)
{
	//ft_printf("FOURTH\n");
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_put_in_my_env(envp, cwd);
	}
	else
	{
		free(*path);
		*path = NULL;
		perror("getcwd");
		return (1);
	}
	return (0);
}

int bt_cd(t_word *args, char ***envp)
{
	char *path;

	path = NULL;
	if (get_cd_path(args, envp, &path))
	{
		return (1);
	}
	if (check_end_token(args, &path, envp))
	{
		return (0);
	}
	if (change_directory(&path, envp))
	{
		return (1);
	}
	if (update_environment(envp, &path))
	{
		return (1);
	}
	if (path)
		free(path);
	return (0);
}
