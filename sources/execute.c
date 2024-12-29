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
		return (ft_strdup(""));
	path_copy = ft_strdup(path_env);
	dir = ft_split(path_copy, ':');
	while (dir[++i])
	{
		full_path = ft_shelljoin(dir[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (ft_free_argvs(dir), full_path);
		}
		free(full_path);
	}
	ft_free_argvs(dir);
	free(path_copy);
	return (ft_strdup(""));
}

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

static int	ft_exec_input(char *input, t_word *orgs, char ***env)
{
	char	*command_path;
	char	**args;
	int		fds[2];
	int		status;
	int		pid;
	int		i;
	int		red;

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	status = 0;
	i = 0;
	red = handle_redirections(orgs, env);
	i = red;
	if (red == -1 || red == -2)
	{
		if (red == -2)
			ft_put_exitcode(env, 2);
		else
			ft_put_exitcode(env, 1);
		reset_fd(fds[0], fds[1]);
		free(input);
		return (1);
	}
	if (has_redir((orgs)->next))
	{
		(orgs)->next = rm_redir_node((orgs)->next);
	}
	if (i)
	{
		if (i == 69)
		{
			ft_put_exitcode(env, 2);
			reset_fd(fds[0], fds[1]);
			ft_print_error(6);
			return (0);
		}
		ft_put_exitcode(env, 1);
		reset_fd(fds[0], fds[1]);
		return (0);
	}
	free(input);
	input = ft_args_to_line(orgs);
	args = ft_split(input, ' ');
	free(input);
	if (!ft_strchr(args[0], '/'))
		command_path = ft_find_command(args[0]);
	else
		command_path = ft_strdup(args[0]);
	pid = fork();
	if (pid == 0)
	{
		if (execve(command_path, args, *env) == -1)
		{
			ft_print_error(0);
			ft_free_argvs(args);
			free(command_path);
			ft_free_all(env, &orgs);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			ft_free_argvs(args);
			free(command_path);
			return (ft_put_exitcode(env, 1), 1);
		}
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 2)
				ft_put_exitcode(env, 2);
			else if (WEXITSTATUS(status) == 0)
				ft_put_exitcode(env, 0);
			else if (WEXITSTATUS(status) == 1)
			{
				ft_put_exitcode(env, 127);
				ft_free_argvs(args);
				free(command_path);
				return (1);
			}
		}
		else
		{
			ft_free_argvs(args);
			free(command_path);
			return (ft_put_exitcode(env, 1), 1);
		}
	}
	ft_free_argvs(args);
	free(command_path);
	reset_fd(fds[0], fds[1]);
	return (0);
}

void	expand_args(t_word *args, char ***envp)
{
	t_word	*temp;
	char	*str;

	temp = args;
	while (temp)
	{
		if (temp->type == ARGUMENT)
		{
			str = expand_string(temp, envp);
			free(temp->value);
			temp->value = str;
		}
		temp = temp->next;
	}
}

int	ft_auto_execute(t_word *args, char ***envp)
{
	char	*input;

	expand_args(args, envp);
	input = ft_args_to_line(args);
	return (ft_exec_input(input, args, envp));
}
