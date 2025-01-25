#include "minishell.h"

void	free_2d_array(char **array)
{
	int	i;

	if (!array)
		return;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array); // Free the outer array
}

char	*merge_filename(t_word *node)
{
	char	*new_value;
	int		i;

	new_value = ft_strdup("");
	while (node->type == ARGUMENT)
	{
		i = 0;
		if (node->value[i] == '"')
			i++;
		while (node->value[i] != '"' && node->value[i])
		{
			new_value = add_char(new_value, node->value[i]);
			i++;
		}
		if (node->_o == 1)
		{
			node = node->next;
		}
		else
			break ;
	}
	return (new_value);
}

void	ft_handle_heredoc_signal(int sigur)
{
	if (sigur == SIGINT)
	{
		//ft_printf("fsdfsdfsd\n");
		//rl_on_new_line();
		//rl_replace_line("", 0);
		//rl_redisplay();
		g_code_of_exit = 130;
		close(STDIN_FILENO);
		//exit(130);
	}
}

/* int	ft_heredoc_signal(void)
{
	if (g_code_of_exit == 130)
	{
		//write(1, "Ctrl+C detected. Exiting heredoc.\n", 35);
		//close(fd);
		//return (open(".heredoc_tmp", O_RDONLY));
		close(STDIN_FILENO);
		return (1);
	}
	return (0);
} */

int	ft_fd_open(int fd)
{
	char dummy[1];

	return ((read(fd, dummy, 0) != -1 || errno != EBADF));
}

/* int heredoc_loop(const char *delimiter, int fd)
{
	char *input;
	//char dummy[1024];
	int i;
	char **lines;

	i = 0;
	signal(SIGINT, ft_handle_heredoc_signal);
	while (1)
	{
		write(1, "> ", 2);
		input = get_next_line(0);
		if (!input)
			break;
		// Split the input into lines in case it includes multiple lines
		lines = ft_split(input, '\n'); // `ft_split` splits by newline, similar to `strtok`
		if ((lines[0] && ft_strncmp(lines[0], delimiter, ft_strlen(lines[0])) == 0))
		{
			//printf("input: %s\tdelimiter: %s\n", input, delimiter);
			//printf("difference: %d\n", ft_strlen(input));
			free_2d_array(lines); // Free the split lines
			//write(fd, "\n", 1); // Ensure newline is preserved
			close(fd);
			return (open(".heredoc_tmp", O_RDONLY)); // Reopen for reading
		}
		// Write the line to the temporary file
		write(fd, input, ft_strlen(input));
		i++;
		free(input);
		if (g_code_of_exit == 130)
		{
			//write(1, "Ctrl+C detected. Exiting heredoc.\n", 35);
			close(fd);
			fd = -1;
			//return (open(".heredoc_tmp", O_RDONLY));
			//close(STDIN_FILENO);
			return (open(".heredoc_tmp", O_RDONLY));
		}
	}
	return(fd);
} */

int heredoc_loop(const char *delimiter, t_word *args, char ***envp, t_heredoc heredoc_vars)
{
    char *input;
    char **lines;

    signal(SIGINT, ft_handle_heredoc_signal); // Install signal handler

    while (1)
    {
        write(1, "> ", 2);
        input = get_next_line(0);
        if (!input)
        {
			ft_free_all(envp, &args);
			free(heredoc_vars.filename);
			free(heredoc_vars.input);
			ft_printf("\n");
            close(heredoc_vars.fd);
            break ;
        }

        lines = ft_split(input, '\n');
        if ((lines[0] && ft_strncmp(lines[0], delimiter, ft_strlen(lines[0])) == 0) || !input)
        {
            free_2d_array(lines);
			ft_free_all(envp, &args);
            free(input);
			free(heredoc_vars.filename);
			free(heredoc_vars.input);
            close(heredoc_vars.fd);
            return open(".heredoc_tmp", O_RDONLY); // Reopen for reading
        }

        write(heredoc_vars.fd, input, ft_strlen(input));
        free(input);
        free_2d_array(lines);
    }

    close(heredoc_vars.fd);
    return	(heredoc_vars.fd);
}

/* int	handle_heredoc(const char *delimiter)
{
	int		fd;
	char	**lines;
	int		i;
	int		pid;

	i = 0;
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc file");
		return (-1);
	}		
	if (ft_strrchr(delimiter, '\n'))
	{	
		lines = ft_split(delimiter, '\n');
		delimiter = lines[i];
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			while (lines[i])
			{
				i++;
				if (!(lines[i]))
				{
					i = heredoc_loop(delimiter, fd);
					free_2d_array(lines);
					return (i);
				}
				if (ft_strcmp(lines[i], delimiter) != 0)
				{
					write(fd, lines[i], ft_strlen(lines[i]));
					write(fd, "\n", 1);
				}
				else
					break;
			}	
		}
		else if (pid > 0)
		{
			signal(SIGINT, SIG_IGN);  // Parent ignores SIGINT to prevent premature termination
			int status;
			waitpid(pid, &status, 0);
		}
		free_2d_array(lines);
		return open(".heredoc_tmp", O_RDONLY); 
	}
	return heredoc_loop(delimiter, fd);
} */

int handle_heredoc(const char *delimiter, t_word *args, char ***envp, t_heredoc heredoc_vars)
{
    pid_t pid;
    int status;

    heredoc_vars.fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if ( heredoc_vars.fd == -1)
    {
        perror("heredoc file");
        return (-1);
    }

    // Fork to handle heredoc
    pid = fork();
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL); // Default behavior for SIGINT in child

        // Run heredoc loop
        heredoc_loop(delimiter, args, envp, heredoc_vars);

        // Close file and exit cleanly
        close(heredoc_vars.fd);
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process
        signal(SIGINT, SIG_IGN); // Ignore SIGINT in parent during heredoc
        waitpid(pid, &status, 0); // Wait for child process to finish
        /* if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            // If child was interrupted by SIGINT, clean up
            printf("Heredoc interrupted by SIGINT\n");
            close(fd);
            unlink(".heredoc_tmp"); // Remove temporary file
            return (-1);
        } */
    }
    else
    {
        // Fork failed
        perror("fork");
        close(heredoc_vars.fd);
        return (-1);
    }

    // Close file descriptor in the parent process and reopen for reading
    close(heredoc_vars.fd);
    return open(".heredoc_tmp", O_RDONLY);
}


int	ft_handle_heredoc(t_word *current, char ***envp, t_word *args, t_heredoc heredoc_vars)
{
	int		fd;
	//char	*filename;

	heredoc_vars.filename = merge_filename(current->next);
	fd = handle_heredoc(current->next->value, args, envp, heredoc_vars);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_put_exitcode(envp, 1);
		close(fd);
		free(heredoc_vars.filename);
		return (-1);
	}
	close(fd);
	free(heredoc_vars.filename);
	return (0);
}

int	ft_check_redir(t_word *args, char ***envp, t_word *current, t_heredoc heredoc_vars)
{
	if (current->type == REDIRECT_OUT)
	{
		if (ft_handle_redirect_out(current, envp))
			return (-1);
	}
	else if (current->type == REDIRECT_APPEND)
	{
		if (ft_handle_redirect_append(current, envp))
			return (-1);
	}
	else if ((current->type == REDIRECT_IN)
		|| ((ft_strchr(current->value, '<') && (current->type != HEREDOC))
			&& ft_strcmp(heredoc_vars.first, "echo")))
	{
		if (ft_handle_redirect_in(current, envp, args))
			return (ft_handle_redirect_in(current, envp, args));
	}
	if (current->type == HEREDOC)
	{
		if (ft_handle_heredoc(current, envp, args, heredoc_vars))
			return (-1);
		//return (-3);
	}
	return (0);
}

int	handle_redirections(t_word *args, char ***envp, t_heredoc heredoc_vars)
{
	t_word	*current;
	t_word	*next;
	//char	*first;

	current = args;
	//first = args->value;
	heredoc_vars.first = args->value;
	while (current)
	{
		if (ft_check_redir(args, envp, current, heredoc_vars))
			return (ft_check_redir(args, envp, current, heredoc_vars));
		next = current->next;
		current = next;
	}
	return (0);
}