#include "minishell.h"

int	handle_heredoc(const char *delimiter)
{
	char	*line;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		if (line[ft_strlen(line) - 1] != '\n')
			write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int	handle_redirections(t_word *args, char ***envp)
{
	t_word	*current;
	t_word	*next;
	int		fd;
	char	*filename;
	char	*first;

	current = args;
	first = args->value;
	while (current)
	{
		if (current->type == REDIRECT_OUT)
		{
			filename = merge_filename(current->next);
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				free(filename);
				if (errno == EACCES)
				{
					ft_print_error(5);
					ft_put_exitcode(envp, 1);
				}
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				ft_put_exitcode(envp, 1);
				close(fd);
				free(filename);
				return (-1);
			}
			close(fd);
			free(filename);
		}
		else if (current->type == REDIRECT_APPEND)
		{
			filename = merge_filename(current->next);
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				ft_print_error(5);
				ft_put_exitcode(envp, 1);
				free(filename);
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				ft_put_exitcode(envp, 1);
				close(fd);
				free(filename);
				return (-1);
			}
			free(filename);
			close(fd);
		}
		else if ((current->type == REDIRECT_IN)
			|| (ft_strchr(current->value, '<')
				&& ft_strcmp(first, "echo")))
		{
			if (!(count_nodes(current) > 2))
				return (69);
			filename = merge_filename(current->next);
			fd = open(filename, O_RDONLY);
			if (fd < 0)
			{
				free(filename);
				if (errno == EACCES)
				{
					ft_print_error(5);
					ft_put_exitcode(envp, 1);
				}
				else if (errno == ENOENT)
				{
					if (!ft_strcmp(current->prev->value, "echo")
						&& (!ft_strncmp(current->next->next->value, "END", 3)
							|| !handle_redirections(current->next->next, envp)))
						return (-1);
					if ((has_pipe(args) && !is_bts_in_pipe(args))
						|| !has_pipe(args))
						ft_print_error(4);
					if (ft_strchr(current->value, '<')
						&& ft_strlen(current->value) > 1)
						return (-2);
					else
						ft_put_exitcode(envp, 1);
					return (-1);
				}
				else
				{
					ft_print_error(5);
					ft_put_exitcode(envp, 1);
				}
				return (-1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				ft_put_exitcode(envp, 1);
				close(fd);
				return (-1);
			}
			close(fd);
			free(filename);
		}
		else if (current->type == HEREDOC)
		{
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
		}
		next = current->next;
		current = next;
	}
	return (0);
}
