#include "minishell.h"

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
		|| (ft_strchr(current->value, '<')
			&& ft_strcmp(first, "echo")))
	{
		if (ft_handle_redirect_in(current, envp, args))
			return (ft_handle_redirect_in(current, envp, args));
	}
	else if (current->type == HEREDOC)
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
	return (0);
}
