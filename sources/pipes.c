#include "minishell.h"

int	execute_piped_command(t_pipes *pipes_struct, char ***envp, t_word *args)
{
	int	pid;
	int	status;

	pid = fork();
	status = 0;
	if (pid == 0)
	{
		if (pipes_struct->command->prev)
			ft_special_free(pipes_struct->command);
		handle_pipe_redirection(pipes_struct->i,
			pipes_struct->pipe_count, pipes_struct->pipes);
		status = is_bt(pipes_struct->command->value,
				pipes_struct->command, envp);
		if (status)
			status = ft_auto_execute(pipes_struct->command, envp);
		ft_free_process_var(pipes_struct);
		ft_free_all(envp, &args);
		exit(status);
	}
	else if (pid < 0)
	{
		perror("fork error");
	}
	return (pid);
}

void	handle_command_execution(t_pipes *pipes_struct, t_word **args,
	char ***envp)
{
	while (pipes_struct->i <= pipes_struct->pipe_count)
	{
		pipes_struct->command = get_next_command(args);
		pipes_struct->pid[pipes_struct->i] = execute_piped_command(pipes_struct,
				envp, *args);
		pipes_struct->status = 0;
		if (pipes_struct->i >= 1 && pipes_struct->pipe_count > 1
			&& pipes_struct->i < pipes_struct->pipe_count)
			pipes_struct->retainer[pipes_struct->i - 1] = pipes_struct->command;
		pipes_struct->i++;
	}
}

void	cleanup_retained_commands(t_pipes *pipes_struct)
{
	if (*pipes_struct->retainer)
	{
		pipes_struct->i = 0;
		while (pipes_struct->i < (pipes_struct->pipe_count - 1))
		{
			ft_free_args(pipes_struct->retainer[pipes_struct->i]);
			pipes_struct->i++;
		}
		ft_free_args(pipes_struct->command);
	}
	else
		ft_free_args(pipes_struct->command);
	pipes_struct->command = NULL;
}

void	wait_for_children(t_pipes *pipes_struct, char ***envp)
{
	int	child_exit_code;

	pipes_struct->i = 0;
	while (pipes_struct->i <= pipes_struct->pipe_count)
	{
		if (waitpid(pipes_struct->pid[pipes_struct->i],
				&pipes_struct->status, 0) == -1)
		{
			perror("waitpid");
			ft_put_exitcode(envp, 1);
		}
		if (WIFEXITED(pipes_struct->status))
		{
			child_exit_code = WEXITSTATUS(pipes_struct->status);
			ft_put_exitcode(envp, child_exit_code);
		}
		pipes_struct->i++;
	}
}

void	pipe_execution(t_word *args, char ***envp)
{
	t_pipes	*pipes_struct;

	ft_init_pipes_struct(&pipes_struct, args);
	handle_command_execution(pipes_struct, &args, envp);
	cleanup_retained_commands(pipes_struct);
	close_pipes(pipes_struct->pipe_count, pipes_struct->pipes);
	wait_for_children(pipes_struct, envp);
	ft_free_process_var(pipes_struct);
}
