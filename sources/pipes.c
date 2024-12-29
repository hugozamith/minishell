#include "minishell.h"

int	execute_piped_command(t_word *command, int i, int pipe_count,
		int pipes[][2], char ***envp, t_word *args,
		int *pidummy, t_word **retainer)
{
	int	pid;
	int	status;

	pid = fork();
	status = 0;
	if (pid == 0)
	{
		if (command->prev)
			ft_special_free(command);
		handle_pipe_redirection(i, pipe_count, pipes);
		status = is_bt(command->value, command, envp);
		if (status)
			status = ft_auto_execute(command, envp);
		ft_free_args(command);
		ft_free_all(envp, &args);
		free(pidummy);
		free(retainer);
		exit(status);
	}
	else if (pid < 0)
	{
		perror("fork error");
	}
	return (pid);
}

void	ft_guarding_args(t_word *args, int i, int pipe_count)
{
	static t_word	*dummy;

	if (i > 1 && i <= pipe_count)
	{
		if (!dummy)
		{
			ft_printf_fd(0, "NOTHING %d\n", i);
			return ;
		}
		ft_pipe_free(dummy);
	}
	dummy = args;
}

void	pipe_execution(t_word *args, char ***envp)
{
	t_word	*command;
	t_word	**retainer;
	int		pipe_count;
	int		status;
	int		*pid;
	int		i;
	int		child_exit_code;

	pipe_count = count_pipes(args);
	int		pipes[pipe_count][2];
	status = 0;
	create_pipes(pipe_count, pipes);
	pid = malloc(sizeof(int) * (pipe_count + 1));
	i = 0;
	retainer = malloc(sizeof(t_word) * (pipe_count + 1));
	retainer[0] = NULL;
	retainer[pipe_count] = NULL;
	while (i <= pipe_count)
	{
		command = get_next_command(&args);
		pid[i] = execute_piped_command(command, i, pipe_count,
				pipes, envp, args, pid, retainer);
		status = 0;
		if (i >= 1 && pipe_count > 1 && i < pipe_count)
		{
			retainer[i - 1] = command;
		}
		i++;
	}
	if (*retainer)
	{
		i = 0;
		while (i < (pipe_count - 1))
		{
			ft_free_args(retainer[i]);
			i++;
		}
		ft_free_args(command);
	}
	else
		ft_free_args(command);
	free(retainer);
	close_pipes(pipe_count, pipes);
	i = 0;
	while (i <= pipe_count)
	{
		if (waitpid(pid[i], &status, 0) == -1)
		{
			perror("waitpid");
			ft_put_exitcode(envp, 1);
		}
		if (WIFEXITED(status))
		{
			child_exit_code = WEXITSTATUS(status);
			ft_put_exitcode(envp, child_exit_code);
		}
		i++;
	}
	free(pid);
}
