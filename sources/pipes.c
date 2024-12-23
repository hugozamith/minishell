#include "minishell.h"

// Count the number of pipes in the argument list
int count_pipes(t_word *args)
{
    int count = 0;
    while (args)
    {
        if (args->type == PIPE)
            count++;
        args = args->next;
    }
    return count;
}

// Get the command before a pipe
t_word *get_next_command(t_word **args)
{
    t_word *command = *args;
	//t_word *next;
	/* t_word *pipe_token;
	pipe_token = NULL; */
    while (*args && (*args)->type != PIPE)
	{
		/* if ((*args)->next->type == PIPE || !(*args)->next)
			break ;
		next = (*args)->next;
		free((*args)->value);
		free(args); */
		//ft_printf("VALUE: %s\n", (*args)->value);
        *args = (*args)->next;
	}
    if (*args && (*args)->type == PIPE)
    {
        t_word *pipe_token = *args;
        *args = (*args)->next;
        pipe_token->next = NULL;
    }
	//printf("command: %s\n", command->value);
	//ft_free_args(pipe_token);
	//ft_printf("A ROUND PASSED\n");
    return command;
}

// Create the pipes for the commands
void create_pipes(int pipe_count, int pipes[][2])
{
    int i = 0;
    while (i < pipe_count)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }
        i++;
    }
	//printf("pipe count\n");
}

// Handle input/output redirection for the pipe execution
void handle_pipe_redirection(int i, int pipe_count, int pipes[][2])
{
    if (i > 0) // Not the first command, needs input from previous pipe
    {
		//ft_printf("first\n");

        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
        {
			//ft_printf_fd(0, "HIHIHIHI\n");
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
    }
    if (i < pipe_count) // Not the last command, needs output to next pipe
    {
		//ft_printf("second\n");
        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
        {
			//ft_printf_fd(0, "HIHIHIHI\n");
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
    }
    // Close all pipe ends in child process
    int j = 0;
    while (j < pipe_count)
    {
		//ft_printf("third\n");
        close(pipes[j][0]);
        close(pipes[j][1]);
        j++;
    }
	/* close(pipes[i][0]);
    close(pipes[i][1]); */
}
char *command_to_str (t_word *command)
{
	char *str = ft_strdup(command->value);
	t_word *temp = command->next;
	while (temp)
	{
	//	if temp
		str = ft_strjoin_free(str, ft_strdup(" "));
		str = ft_strjoin_free(str, temp->value);
		temp = temp->next;
	}
	return (str);
}

// Close all pipes after fork
void close_pipes(int pipe_count, int pipes[][2])
{
    int i = 0;
    while (i < pipe_count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

// Execute a command with pipes
int execute_piped_command(t_word *command, int i, int pipe_count, int pipes[][2], char ***envp, t_word *args, int *pidummy)
{
	/* if (i == pipe_count)
		if (!args)
			ft_printf_fd(0, "HEREEEE\n"); */
	int pid = fork();
	int status = 0;
	(void) pipe_count;
	(void) i;
	(void) pipes;

    if (pid == 0) // Child process
    {
		//write(1, "oi", 2);
		//ft_printf_fd(0, "HEREEEE\n");
		//ft_free_args(args);
        handle_pipe_redirection(i, pipe_count, pipes);
		//ft_printf_fd(0, "HEREEEE\n");
		//ft_printf("--%s--\n", args->next->value);
		status = is_bt(command->value, command, envp);
        if (status)
            status = ft_auto_execute(command, envp);
		/* write(1, "hola\n", 5);
		ft_printf("--%s--\n", args->next->value);
		ft_printf("ooooo"); */
		//ft_printf_fd(0, "HERE\n");
		ft_free_args(command);
		/* if (i == 0)
		{
			if (command->prev)
				while (command->prev)
					command = command->prev;
			ft_free_args(command);
		}
		else
			ft_free_args(command); */
		/* if (i == 0)
		{ */
		//ft_printf_fd(0, "-------%s--\n", args->next->value);
		/* if (i == pipe_count)
			if (args->prev)
			{
				while (args->prev)
					args = args->prev;
				ft_free_all(envp, &args);
				ft_printf_fd(0, "HEREEEE\n");
			} */
		ft_free_all(envp, &args);
		free(pidummy);
			//ft_printf_fd(0, "HEREEEE\n");
		/* } else if (i == pipe_count)
		{
			while (args->prev != NULL)
				args = args->prev;
			ft_printf_fd(0, "--%s--\n", args->next->value);
			ft_free_all(envp, &args);
			free(pidummy);
		} */
		exit(status);
    }
    else if (pid < 0)
	{
        perror("fork error");
		//ft_put_exitcode(envp, 1);
	}
	return (pid);
}
/* 
void	ft_free_prev_args(void)
{
	ft_printf_fd(0, "Im here!!\n");
} */

// Execute the full pipeline
void pipe_execution(t_word *args, char ***envp)
{
	t_word *command;
    int pipe_count = count_pipes(args);
	int status = 0;
	//t_word *first_value = args;
    //printf("pipe count: %d\n", pipe_count);
    int pipes[pipe_count][2];
    create_pipes(pipe_count, pipes);
	int *pid = malloc(sizeof(int) * (pipe_count + 1));
    int i = 0;
    while (i <= pipe_count)
    {
		/* if (i == 0)
			command = args;
		else */
		command = get_next_command(&args);
		//ft_free_prev_args();
		/* if (i == 0)
			break ; */
			//ft_printf_fd(0, "NOTHING TO SEE HERE\n");
		//command = get_next_command(&args);
		//ft_free_all(NULL, &args);
		//if (pipe > 0)
		//ft_printf_fd(0, "HERERERE: %s", args->next->value);
        pid[i] = execute_piped_command(command, i, pipe_count, pipes, envp, args, pid);
		//printf("i: %d\n", i);
		//ft_printf("HELLO\n");
		status = 0;
		//*pid = 0;
        i++;
		//free(command);
		//ft_free_args(args);
		/* while (command->prev)
			command = command->prev;
		ft_free_args(command); */
    }
	//ft_free_args(args);
	/* while (i != 0)
	{ 
		ft_printf("VALUE: %d\n", pid[i]);
		if (waitpid(pid[i], &status, 0) == -1) {
            perror("waitpid");
            ft_put_exitcode(envp, 1); // Return a failure exit code
    	}
		i--;
	} */
    close_pipes(pipe_count, pipes);

    // Wait for all child processes
    i = 0;
    while (i <= pipe_count)
    {
        //wait(NULL);
		//ft_printf("VALUE: %d\n", pid[i]);
		if (waitpid(pid[i], &status, 0) == -1) {
            perror("waitpid");
            ft_put_exitcode(envp, 1); // Return a failure exit code
    	}
		if (WIFEXITED(status))
        {
            int child_exit_code = WEXITSTATUS(status);
            //printf("Parent: Child exited with code %d\n", child_exit_code);

            // Pass the child's exit code to ft_put_exitcode
            ft_put_exitcode(envp, child_exit_code);
        }
        i++;
	}
	free(pid);
}
