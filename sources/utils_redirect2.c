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

int	ft_bad_fd_in(t_word *current, char ***envp, t_word *args, char *filename)
{
	t_heredoc	nothing;

	nothing.first = NULL;
	free(filename);
	if (errno == EACCES)
		ft_bad_fd_error(envp);
	else if (errno == ENOENT)
	{
		if (current->prev && !ft_strcmp(current->prev->value, "echo")
			&& (!ft_strncmp(current->next->next->value, "END", 3)
				|| !handle_redirections(current->next->next, envp, nothing)))
			return (ft_print_error(4), -1);
		if ((has_pipe(args) && !is_bts_in_pipe(args)) || !has_pipe(args))
			ft_print_error(4);
		if (ft_strchr(current->value, '<') && ft_strlen(current->value) > 1)
			return (-2);
		else
			ft_put_exitcode(envp, 1);
		return (-1);
	}
	else
		ft_print_this_error(envp);
	return (-1);
}

int	ft_handle_redirect_out(t_word *current, char ***envp)
{
	int		fd;
	char	*filename;

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
		return (close(fd), free(filename), -1);
	}
	close(fd);
	free(filename);
	return (0);
}

int	ft_handle_redirect_append(t_word *current, char ***envp)
{
	int		fd;
	char	*filename;

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
	return (0);
}

int	ft_handle_redirect_in(t_word *current, char ***envp, t_word *args)
{
	int		fd;
	char	*filename;
	t_word	*dummy;

	if (!(count_nodes(current) > 2))
		return (69);
	dummy = current;
	while (ft_strncmp(dummy->value, "<", 1))
	{
		dummy = dummy->next;
	}
	filename = merge_filename(dummy->next);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_bad_fd_in(current, envp, args, filename));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_put_exitcode(envp, 1);
		close(fd);
		return (-1);
	}
	close(fd);
	free(filename);
	return (0);
}
