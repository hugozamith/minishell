#include "minishell.h"

static char	*ft_shelljoin(char *dir, char *command)
{
	int		dirlen;
	int		comlen;
	int		len;
	char	*result;

	dirlen = ft_strlen(dir);
	comlen = ft_strlen(command);
	len = dirlen + comlen;
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	ft_memcpy(result, dir, dirlen);
	result[dirlen] = '/';
	ft_memcpy(result + dirlen + 1, command, comlen);
	result[len + 1] = '\0';
	return (result);
}

static char	*ft_find_command(char *command)
{
	char	*path_env;
	char	*path_copy;
	char	*full_path;
	char	**dir;
	int		i;

	i = -1;
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	dir = ft_split(path_copy, ':');
	while (dir[++i])
	{
		full_path = ft_shelljoin(dir[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (ft_free_argvs(dir),free(full_path), full_path);
		}
		free(full_path);
	}
	ft_free_argvs(dir);
	free(path_copy);
	return (NULL);
}

char	*ft_args_to_line(t_word *args)
{
	char	*result;
	char	*old_str;
	char	*expand_str;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (args->type == COMMAND || args->type == ARGUMENT)
	{
		old_str = result;
		expand_str = expand_string(args);
		result = ft_strjoin(result, expand_str);
		free(old_str);
		old_str = result;
		result = ft_strjoin(result, " ");
		free(old_str);
		free(expand_str);
		args = args->next;
	}
	return (result);
}

static void	ft_exec_input(char *input)
{
	char	*command_path;
	char	**args;

	args = ft_split(input, ' ');
	free(input);
	if (!ft_strchr(args[0], '/'))
		command_path = ft_find_command(args[0]);
	else
		command_path = args[0];
	if (fork() == 0)// Child process
	{
		if (execve(command_path, args, __environ) == -1)
		{
			ft_printf("%s: command not found\n", args[0]);
			ft_free_argvs(args);
			//free(input);
			exit(EXIT_FAILURE);
		}
	}
	else // Parent process
		wait(NULL); // Wait for the child process to finish
 		//free(input);
	ft_free_argvs(args);
	//free(input);
}

void	ft_auto_execute(t_word *args)
{
	char *input;

	//ft_printf("BEFORE: %s\n", args->value);
	input = ft_args_to_line(args);
	//ft_printf("AFTER: %s\n", input);
	ft_exec_input(input);
}
