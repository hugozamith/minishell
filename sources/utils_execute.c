#include "minishell.h"

char	*ft_args_to_line(t_word *args)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (args->type == COMMAND || args->type == ARGUMENT)
	{
		result = ft_strjoin_free(result, ft_strdup(args->value));
		result = ft_strjoin_free(result, ft_strdup(" "));
		args = args->next;
	}
	return (result);
}

void	ft_exit_failure(char *command_path, char **args,
	char ***env, t_word *orgs)
{
	ft_print_error(0);
	ft_free_argvs(args);
	free(command_path);
	ft_free_all(env, &orgs);
	exit(EXIT_FAILURE);
}

int	ft_handle_exit_status(int status, int exit_code, char ***env)
{
	exit_code = WEXITSTATUS(status);
	if (exit_code == 2)
		ft_put_exitcode(env, 2);
	else if (exit_code == 0)
		ft_put_exitcode(env, 0);
	else
		ft_put_exitcode(env, 127);
	if (exit_code != 0 && exit_code != 2)
		return (1);
	return (0);
}

char	*prepare_command_and_args(t_word *orgs, char ***env, char ***args)
{
	char	*input;

	input = ft_args_to_line(orgs);
	*args = ft_split(input, ' ');
	free(input);
	if (!ft_strchr((*args)[0], '/'))
		return (ft_find_command((*args)[0], env));
	return (ft_strdup((*args)[0]));
}

char	*ft_find_command(char *command, char ***env)
{
	char	*path_env;
	char	*path_copy;
	char	*full_path;
	char	**dir;
	int		i;

	i = -1;
	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (ft_strdup(""));
	path_copy = ft_strdup(path_env);
	dir = ft_split(path_copy, ':');
	while (dir[++i])
	{
		full_path = ft_shelljoin(dir[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			free(path_env);
			return (ft_free_argvs(dir), full_path);
		}
		free(full_path);
	}
	ft_free_argvs(dir);
	return (free(path_copy), free(path_env), ft_strdup(""));
}
