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
		if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND || current->type == HEREDOC)
		{
			if (current->prev->type == COMMAND)
			{
				while (current->next->O == 1)
					current = current->next;	
				return (current->next->next);			
			}
			while (current->next->O == 1)
				current = current->next;
			current->prev->next = current->next->next;
			return (current->prev);
		}
		next = current->next;
		/* free(current->value);
		free(current); */
		current = next;
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
	int		redir;

	//ft_printf("HEREE\n");
	if (ft_just_exit_code(args))
	{
		//write(1, "JUST_EXIT_CODE\n", 16);
		expanded = expand_string(args->next, envp);
		if (*expanded == '1' || *expanded == '2')
		{
			ft_printf("%s\n", expanded);
			free(expanded);
			return (0);
		}
		free(expanded);
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
	//ft_printf("HERE\n");
	//ft_printf("VALUE: %s\n", args->value);
    if (handle_redirections(args, envp) < 0)
    {
		//ft_printf("IN HERE\n");
        ft_put_exitcode(envp, 1);
		reset_fd(fds[0], fds[1]);
        return (1);
    }

    current = args->next;
    // Handle any redirection
//	while (tokensrch(args, REDIRECT_OUT) || tokensrch(args->prev, REDIRECT_OUT))
//		args = args->next;

    // Remove all redirection nodes from the argument list
    if (has_redir(current))
	{
		redir = 1;
		current = rm_redir_node(current);
		//ft_printf_fd(0, "VALUE after: %s\n", current->value);
	}
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
	//ft_printf_fd(0, "HERE!\n");
	//free(current);
	/* if (args->prev)
		ft_printf_fd(0, "I CAN GO BACK!\n"); */
	//ft_printf_fd(0, "VALUE before: %s\n", token_type_to_str(current->prev->prev->type));
	if (current->prev->prev->type == REDIRECT_OUT && redir)
	{
		while (current->type != REDIRECT_OUT && current->type != REDIRECT_APPEND && current->type != REDIRECT_IN)
			current = current->prev;
		//ft_put_exitcode(envp, 0);
		//ft_printf_fd(0, "VALUE after: %s\n", current->value);
		while (current->type != END)
		{
			t_word *next = current->next;
			free(current->value);
			free(current);
			current = next;
		}
	}
	//current = current->prev;
	//ft_printf_fd(0, "VALUE after: %s\n", current->value);
    return (0);
}
