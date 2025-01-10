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

int heredoc_loop(const char *delimiter, int fd)
{
	char *input;
	int i;
	char **lines;

	i = 0;
	while (1)
	{
		write(1, "> ", 2);
		input = get_next_line(0);
		if (!input)
			break;
		// Split the input into lines in case it includes multiple lines
		lines = ft_split(input, '\n'); // `ft_split` splits by newline, similar to `strtok`
		if (ft_strcmp(input, delimiter) == 0)
		{
			free_2d_array(lines); // Free the split lines
			//write(fd, "\n", 1); // Ensure newline is preserved
			close(fd);
			return open(".heredoc_tmp", O_RDONLY); // Reopen for reading
		}
		// Write the line to the temporary file
		write(fd, input, ft_strlen(input));
		i++;
		free(input);
	}
	return(fd);
}

int	handle_heredoc(const char *delimiter)
{
	int		fd;
	char	**lines;
	int		i;

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
		free_2d_array(lines);
		return open(".heredoc_tmp", O_RDONLY); 
	}
	return heredoc_loop(delimiter, fd);
}

int	ft_handle_heredoc(t_word *current, char ***envp)
{
	int		fd;
	char	*filename;

	filename = merge_filename(current->next);
	fd = handle_heredoc(current->next->value);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_put_exitcode(envp, 1);
		close(fd);
		free(filename);
		return (-1);
	}
	close(fd);
	free(filename);
	return (0);
}

int	ft_check_redir(t_word *args, char ***envp, t_word *current, char *first)
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
			&& ft_strcmp(first, "echo")))
	{
		if (ft_handle_redirect_in(current, envp, args))
			return (ft_handle_redirect_in(current, envp, args));
	}
	//printf("value: %s\n", token_type_to_str(current->type));
	if (current->type == HEREDOC)
	{
		if (ft_handle_heredoc(current, envp))
			return (-1);	
	}
	return (0);
}

int	handle_redirections(t_word *args, char ***envp)
{
	t_word	*current;
	t_word	*next;
	char	*first;

	current = args;
	first = args->value;
	while (current)
	{
		if (ft_check_redir(args, envp, current, first))
			return (ft_check_redir(args, envp, current, first));
		next = current->next;
		current = next;
	}
	//write(1, "imhere\n", 7);
	return (0);
}