#include "minishell.h"

int	heredoc_loop(const char *delimiter, t_heredoc heredoc_vars)
{
	char	*input;
	char	**lines;
	char	**str_list;

	str_list = ft_list_init();
	signal(SIGINT, ft_handle_heredoc_signal);
	while (++heredoc_vars.i)
	{
		write(0, "> ", 2);
		input = get_next_line(0);
		if (!input)
		{
			ft_free_heredoc(str_list, NULL, heredoc_vars, NULL);
			break ;
		}
		lines = ft_split(input, '\n');
		if (!lines[0] || ft_strncmp(lines[0], delimiter, 50))
			str_list = ft_new_list(str_list, heredoc_vars.i, input);
		if ((lines[0] && ft_strncmp(lines[0], delimiter, 100) == 0)
			|| !input)
			return (ft_free_heredoc(str_list, lines, heredoc_vars, input));
		ft_free_it(NULL, lines, input);
	}
	return (ft_close_it(heredoc_vars.fd), heredoc_vars.fd);
}

int	handle_heredoc(const char *delimiter, t_heredoc heredoc_vars)
{
	pid_t	pid;
	int		status;

	heredoc_vars.fd = ft_get_fd(heredoc_vars);
	if (heredoc_vars.fd == -1)
		return (perror("heredoc file"), -1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_loop(delimiter, heredoc_vars);
		if (heredoc_vars.fd != -2)
			close(heredoc_vars.fd);
		exit(0);
	}
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
	}
	else
		return (perror("fork"), ft_close_it(heredoc_vars.fd), -1);
	if (heredoc_vars.fd != -2)
		close(heredoc_vars.fd);
	return (open(".heredoc_tmp", O_RDONLY));
}

int	ft_handle_heredoc(t_word *current, char ***envp, t_word *args,
			t_heredoc heredoc_vars)
{
	int	fd;

	heredoc_vars.filename = ft_get_filename(current);
	heredoc_vars.env = envp;
	heredoc_vars.args = args;
	heredoc_vars.i = 0;
	fd = handle_heredoc(current->next->value, heredoc_vars);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_put_exitcode(envp, 1);
		if (fd != -2)
			close(fd);
		free(heredoc_vars.filename);
		return (-1);
	}
	if (fd != -2)
		close(fd);
	free(heredoc_vars.filename);
	return (1);
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
		|| ((ft_strchr(current->value, '<')
				&& (current->type != HEREDOC))
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
