#include "minishell.h"

int	heredoc_loop(const char *delimiter, t_word *args,
	char ***envp, t_heredoc heredoc_vars)
{
	char	*input;
	char	**lines;

	signal(SIGINT, ft_handle_heredoc_signal);
	while (1)
	{
		write(1, "> ", 2);
		input = get_next_line(0);
		if (!input)
		{
			ft_free_heredoc(args, envp, heredoc_vars, input);
			break ;
		}
		lines = ft_split(input, '\n');
		if ((lines[0] && ft_strncmp(lines[0], delimiter,
					ft_strlen(lines[0])) == 0) || !input)
			return (free_2d_array(lines),
				ft_free_heredoc(args, envp, heredoc_vars, input),
				open(".heredoc_tmp", O_RDONLY));
		write(heredoc_vars.fd, input, ft_strlen(input));
		free(input);
		free_2d_array(lines);
	}
	close(heredoc_vars.fd);
	return (heredoc_vars.fd);
}

int	handle_heredoc(const char *delimiter, t_word *args,
	char ***envp, t_heredoc heredoc_vars)
{
	pid_t	pid;
	int		status;

	heredoc_vars.fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_vars.fd == -1)
		return (perror("heredoc file"), -1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_loop(delimiter, args, envp, heredoc_vars);
		close(heredoc_vars.fd);
		exit(0);
	}
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
	}
	else
		return (perror("fork"), close(heredoc_vars.fd), -1);
	close(heredoc_vars.fd);
	return (open(".heredoc_tmp", O_RDONLY));
}

int	ft_handle_heredoc(t_word *current, char ***envp, t_word *args,
	t_heredoc heredoc_vars)
{
	int		fd;

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

int	ft_check_redir(t_word *args, char ***envp, t_word *current,
	t_heredoc heredoc_vars)
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
	}
	return (0);
}

int	handle_redirections(t_word *args, char ***envp, t_heredoc heredoc_vars)
{
	t_word	*current;
	t_word	*next;

	current = args;
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
