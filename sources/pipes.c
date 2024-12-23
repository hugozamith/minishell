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
    while (*args && (*args)->type != PIPE)
        *args = (*args)->next;

    if (*args && (*args)->type == PIPE)
    {
        t_word *pipe_token = *args;
        *args = (*args)->next;
        pipe_token->next = NULL;
    }
	//printf("command: %s\n", command->value);
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
        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
        {
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
    }
    if (i < pipe_count) // Not the last command, needs output to next pipe
    {
        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
        {
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
    }
    // Close all pipe ends in child process
    int j = 0;
    while (j < pipe_count)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
        j++;
    }
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


// Execute a command with pipes
int execute_piped_command(t_word *command, int i, int pipe_count, int pipes[][2], char ***envp)
{
	//ft_printf("FIRST\n");
    /* ft_printf("COMMAND: %s\n", command->value);
	t_word *temp = command;
	while (temp)
	{
		printf("Token Type: %s, Value: %s\n", token_type_to_str(temp->type), temp->value);
		temp = temp->next;
	} */
	int pid = fork();
	int status = 0;
    if (pid == 0) // Child process
    {
		//printf("\nAAAAAA\n");
        handle_pipe_redirection(i, pipe_count, pipes);
		//printf("command: %s\n", command->value);
		status = is_bt(command->value, command, envp);
		//printf("VALUE1: %d\n", status);
        if (status)
            status = ft_auto_execute(command, envp);
		//free(line);
		//printf("VALUE2: %d\n", status);
        exit(status);
		//ft_put_exitcode(envp, 1);
    }
    else if (pid < 0)
	{
        perror("fork error");
		//ft_put_exitcode(envp, 1);
	}
	/* else if (pid == 0)
		printf("VALUE after: %d\n", status); */
	/* else // Parent process
    {
		// Wait for the child to finish and retrieve its exit status
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            ft_put_exitcode(envp, 1);
            return;
        }

        // WIFEXITED checks if the child exited normally
        if (WIFEXITED(status))
        {
            int child_exit_code = WEXITSTATUS(status);
            printf("Parent: Child exited with code %d\n", child_exit_code);

            // Pass the child's exit code to ft_put_exitcode
            ft_put_exitcode(envp, child_exit_code);
        }
        else
        {
            printf("Parent: Child did not exit normally\n");
            ft_put_exitcode(envp, 1);
        }
	} */
	//reset_fd(pipes[0], pipes[1]);
	//ft_put_exitcode(envp, status);
	return (pid);
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

// Execute the full pipeline
void pipe_execution(t_word *args, char ***envp)
{
    int pipe_count = count_pipes(args);
	int status = 0;
    //printf("pipe count: %d\n", pipe_count);
    int pipes[pipe_count][2];
    create_pipes(pipe_count, pipes);
	int *pid = malloc(sizeof(int) * pipe_count);
    int i = 0;
    while (i <= pipe_count)
    {
        t_word *command = get_next_command(&args);
        pid[i] = execute_piped_command(command, i, pipe_count, pipes, envp);
		//printf("i: %d\n", i);
		//ft_printf("HELLO\n");
		status = 0;
		//*pid = 0;
        i++;
    }
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
}
