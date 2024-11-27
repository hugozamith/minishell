#include "minishell.h"

int tokensrch(t_word *args, t_tokens token)
{
	while (args)
	{
		if (args->type == token)
			return (1);
		args = args->next;
	}
	return (0);
}

int	ft_just_exit_code(t_word *args)
{
	//printf("args->value: %s\n", args->value);
	if (!ft_strcmp(args->value, "echo") 
			&& !ft_strcmp(args->next->value, "$?")
			&& !ft_strcmp(args->next->next->value, "END"))
		return (1);
	return(0);
}

t_word *rm_redir_node(t_word *args)
{
	t_word *current;
	t_word *next;

	current = args;
	while (current)
	{
		next = current->next;
		if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND || current->type == HEREDOC)
		{
			if (current->prev)
				current->prev->next = current->next->next;
			if (current->next->next)
				current->next->next->prev = current->prev;
			free(current->value);
			free(current);
			return (next);
		}
		current = current->next;
	}
	return (args);
}

int has_redir(t_word *args)
{
	while (args)
	{
		if (args->type == REDIRECT_IN || args->type == REDIRECT_OUT
			|| args->type == REDIRECT_APPEND || args->type == HEREDOC)
			return (1);
		args = args->next;
	}
	return (0);
}

int bt_echo(t_word *args, int fd, char ***envp)
{
    int     newline;
    t_word  *current;
    char    *expanded;
    int     fds[2];

	if (ft_just_exit_code(args))
	{
		//write(1, "JUST_EXIT_CODE\n", 16);
		expanded = expand_string(args->next, envp);
		if (*expanded == '1')
		{
			ft_printf("%s\n", expanded);
			free(expanded);
			return (0);
		}
	}
	//write(1, "NOT_JUST_EXIT_CODE\n", 20);
    // Save the original file descriptors
    fds[0] = dup(STDIN_FILENO);
    fds[1] = dup(STDOUT_FILENO);
    if (fds[0] == -1 || fds[1] == -1)
    {
        perror("dup failed");
		ft_put_exitcode(envp, 1);
        return (1);
    }

    newline = 1;
    current = args->next;

    // Handle any redirection
//	while (tokensrch(args, REDIRECT_OUT) || tokensrch(args->prev, REDIRECT_OUT))
//		args = args->next;
    if (handle_redirections(args) < 0)
    {
        ft_put_exitcode(envp, 1);
        return (1);
    }

    // Remove all redirection nodes from the argument list
    while (has_redir(args))
        args = rm_redir_node(args);
	//printf("current->type: %s\n", token_type_to_str(current->type));



	//printf("current->value: %s\n", current->value);
    // If there's an error in redirection, return
    if (fd < 0)
    {
        reset_fd(fds[0], fds[1]);
		ft_put_exitcode(envp, 1);
        return (1);
    }

    // Check if we need to print without a newline
    if (current && ft_strncmp(current->value, "-n", ft_strlen(current->value)) == 0)
    {
        newline = 0;
        current = current->next;
    }
	
	ft_put_exitcode(envp, 0);
    // Print all arguments with expansion
    while (current && current->type == ARGUMENT)
    {
		//printf("current->value: %s\n", current->value);
        expanded = expand_string(current, envp);
        ft_putstr_fd(expanded, fd);
        free(expanded);

        if (current->next && current->next->type != END && current->next->type == ARGUMENT &&
            !(current->next->value[0] == '\'' || current->next->value[0] == '"') &&
            !(current->value[0] == '\'' || current->value[0] == '"'))
        {
            ft_putchar_fd(' ', fd);
        }

        current = current->next;

    }

    // Print newline if required
    if (newline)
	{
        ft_putchar_fd('\n', fd);
	}

    // Close file descriptor if it's not STDOUT
	if (fd != STDOUT_FILENO)
        close(fd);

    // Reset file descriptors to their original state
    reset_fd(fds[0], fds[1]);
    return (0);
}
