#include "minishell.h"

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
	if (!ft_strncmp(command, "/", 1))
		path_env = getenv("PATH");
	else
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

int	ft_sum_exit_code(t_word **args)
{
	int		nbr;
	int		result;
	t_word	*temp;

	nbr = -1;
	result = 0;
	temp = *args;
	while (temp)
	{
		if (!ft_strncmp(temp->value, "$?", 2))
		{
			if (ft_strncmp(temp->value, "$?", 3))
				return (-1);
			if (nbr == -1)
				nbr = (ft_put_exitcode(NULL, -2) != 0);
			else
				nbr = (g_code_of_exit != 0);
			result += nbr;
		}
		temp = temp->next;
	}
	ft_printf("%d\n", result);
	return (ft_put_exitcode(NULL, result), result);
}

int	ft_only_redir(t_word *args)
{
	if ((args->type == REDIRECT_APPEND || args->type == REDIRECT_IN
		|| args->type == REDIRECT_OUT || args->type == HEREDOC) && args->next->type == END)
	{
		ft_print_error(11);
		return (1);
	}
	return (0);
}