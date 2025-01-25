#include "minishell.h"

char	*ft_shelljoin(char *dir, char *command)
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

static int	handle_redirections_and_errors(t_word *orgs, char ***env,
	int *fds, char *input)
{
	int			red;
	t_heredoc	heredoc_vars;

	heredoc_vars.input = input;
	red = handle_redirections(orgs, env, heredoc_vars);
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
	if (has_redir(orgs->next))
		orgs->next = rm_redir_node(orgs->next);
	if (red == 69)
	{
		ft_put_exitcode(env, 2);
		reset_fd(fds[0], fds[1]);
		ft_print_error(6);
		return (1);
	}
	return (0);
}

static int	execute_command(char *command_path, char **args,
	char ***env, t_word *orgs)
{
	int	pid;
	int	status;
	int	exit_code;

	pid = fork();
	exit_code = 0;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(command_path, args, *env) == -1)
			ft_exit_failure(command_path, args, env, orgs);
	}
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		if (waitpid(pid, &status, 0) == -1)
			return (perror("waitpid"), ft_put_exitcode(env, 1), 1);
		if (WIFEXITED(status))
			if (ft_handle_exit_status(status, exit_code, env))
				return (1);
		if (WIFSIGNALED(status))
			ft_seg_fault(WTERMSIG(status));
	}
	return (0);
}

static int	ft_exec_input(char *input, t_word *orgs, char ***env)
{
	int		fds[2];
	char	**args;
	char	*command_path;

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
		return (perror("dup"), 1);
	if (handle_redirections_and_errors(orgs, env, fds, input))
		return (1);
	free(input);
	command_path = prepare_command_and_args(orgs, env, &args);
	if (execute_command(command_path, args, env, orgs))
	{
		if (args)
			ft_free_argvs(args);
		free(command_path);
		reset_fd(fds[0], fds[1]);
		return (1);
	}
	if (args)
		ft_free_argvs(args);
	free(command_path);
	reset_fd(fds[0], fds[1]);
	return (0);
}

int	ft_auto_execute(t_word *args, char ***envp)
{
	char	*input;

	if (!ft_strncmp(args->value, "expr", 5))
	{
		if (ft_sum_exit_code(&args) != -1)
			return (0);
	}
	if (ft_only_redir(args))
		return (0);
	expand_args(args, envp);
	if (!ft_strncmp(args->value, "<", 1) && !has_pipe(args))
		input = ft_special_args_to_line(args);
	else if (!ft_strncmp(args->value, "<", 1) && has_pipe(args))
		return (1);
	else if (!ft_strncmp(args->value, ">", 1) && args->next->next->type == END)
		return (ft_just_create(args), 0);
	else
		input = ft_args_to_line(args);
	return (ft_exec_input(input, args, envp));
}
