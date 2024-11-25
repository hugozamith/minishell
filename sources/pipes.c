#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void execute_piped_command(t_word **command, int i, int pipe_count, int pipes[][2], char ***envp)
{
    int pid = fork();
    if (pid == 0) // Child process
    {
		//printf("\nAAAAAA\n");
        handle_pipe_redirection(i, pipe_count, pipes);
		//printf("command: %s\n", command->value);
        if (is_bt((*command)->value, *command, envp))
            ft_auto_execute(*command, envp);
		ft_free_all(envp, command);
		//free(line);
        exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
	{
    	perror("fork error");
	}
	else
	{
		wait(NULL);
	}
	//ft_free_args(command);
	//ft_free_all(envp, &command);
	//ft_free_env(envp);
	//printf("Command-value: %s\n", (*command)->value);
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
	t_word *command;
    int pipe_count = count_pipes(args);
    //printf("pipe count: %d\n", pipe_count);
    int pipes[pipe_count][2];
    create_pipes(pipe_count, pipes);
	//printf("pipe count\n");
    int i = 0;
    while (i <= pipe_count - 1)
    {

		//execute_daddy(&args, i, pipe_count, pipes, envp);
        command = get_next_command(&args);
        execute_piped_command(&command, i, pipe_count, pipes, envp);
			//printf("i: %d\n", i);
        i++;
    }
    close_pipes(pipe_count, pipes);

    // Wait for all child processes
    i = 0;
    while (i <= pipe_count)
    {
        wait(NULL);
        i++;
    }
}
