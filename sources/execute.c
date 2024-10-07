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
		return NULL;
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
		return NULL;
	path_copy = ft_strdup(path_env);
	dir = ft_split(path_copy, ':');
	while (dir[++i])
	{
		full_path = ft_shelljoin(dir[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
	}
	free(path_copy);
	return (NULL);
}

static void	ft_exec_input(char *input)
{
	char *command_path;
	char **args;

	args = ft_split(input, ' ');
	command_path = ft_find_command(args[0]);
	if (fork() == 0)// Child process
	{
        if (execve(command_path, args, NULL) == -1) {
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
    } else // Parent process
        wait(NULL); // Wait for the child process to finish
    //free(input);
	//command_path = ft_find_command(word);
}
void	ft_auto_execute(char *input)
{
	char **str;
	str = ft_split(input, '|');
	while (*str)
	{
		ft_exec_input(*str);
		//ft_printf("STR: %s", *str);
		str++;
	}
}