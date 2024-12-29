#include "minishell.h"

int	tokensrch(t_word *args, t_tokens token)
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
	if (!ft_strcmp(args->value, "echo")
		&& !ft_strcmp(args->next->value, "$?")
		&& !ft_strcmp(args->next->next->value, "END"))
		return (1);
	return (0);
}

t_word	*rm_redir_node(t_word *args)
{
	t_word	*current;
	t_word	*next;
	t_word	*prev;

	current = args;
	while (current)
	{
		next = current->next;
		if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND || current->type == HEREDOC)
		{
			if (current->prev->type == COMMAND)
			{
				while (current->next->O == 1)
					current = current->next;
				prev = current->prev;
				current->prev->next = current->next->next;
				return (ft_special_node_free(&current), prev->next);
			}
			while (current->next->O == 1)
				current = next;
			prev = current->prev;
			current->prev->next = current->next->next;
			return (ft_special_node_free(&current), prev);
		}
		current = next;
	}
	return (args);
}

int	has_redir(t_word *args)
{
	while (args)
	{
		if (args->type == REDIRECT_IN || args->type == REDIRECT_OUT
			|| args->type == REDIRECT_APPEND || args->type == HEREDOC)
		{
			if (args->type == REDIRECT_OUT)
				return (2);
			else
				return (1);
		}
		args = args->next;
	}
	return (0);
}

int	bt_echo(t_word *args, int fd, char ***envp)
{
	int		newline;
	t_word	*current;
	char	*expanded;
	int		fds[2];
	int		i;
	int		redir;

	i = 0;
	if (ft_just_exit_code(args))
	{
		expanded = expand_string(args->next, envp);
		if (*expanded == '1' || *expanded == '2')
		{
			ft_printf("%s\n", expanded);
			free(expanded);
			return (0);
		}
		free(expanded);
	}
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
	{
		perror("dup failed");
		ft_put_exitcode(envp, 1);
		return (1);
	}
	newline = 1;
	i = handle_redirections(args, envp);
	if (i < 0)
	{
		ft_put_exitcode(envp, 1);
		reset_fd(fds[0], fds[1]);
		return (1);
	}
	if (i == 69)
	{
		ft_put_exitcode(envp, 2);
		reset_fd(fds[0], fds[1]);
		ft_print_error(6);
		return (0);
	}
	current = args->next;
	redir = has_redir(current);
	if (redir)
	{
		current = rm_redir_node(current);
	}
	if (fd < 0)
	{
		reset_fd(fds[0], fds[1]);
		ft_put_exitcode(envp, 1);
		return (1);
	}
	if (current && ft_strncmp(current->value, "-n",
			ft_strlen(current->value)) == 0)
	{
		newline = 0;
		current = current->next;
	}
	ft_put_exitcode(envp, 0);
	while (current && current->type == ARGUMENT)
	{
		expanded = expand_string(current, envp);
		ft_putstr_fd(expanded, fd);
		free(expanded);
		if (current->next && current->next->type != END
			&& current->next->type == ARGUMENT
			&& !(current->next->value[0] == '\''
				|| current->next->value[0] == '"')
			&& !(current->value[0] == '\'' || current->value[0] == '"'))
		{
			ft_putchar_fd(' ', fd);
		}
		current = current->next;
	}
	if (newline)
	{
		ft_putchar_fd('\n', fd);
	}
	if (fd != STDOUT_FILENO)
		close(fd);
	reset_fd(fds[0], fds[1]);
	if (redir == 3)
	{
		ft_redirect_free(current, envp);
	}
	return (0);
}
