#include "minishell.h"

void	free_2d_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_handle_heredoc_signal(int sigur)
{
	if (sigur == SIGINT)
	{
		g_code_of_exit = 130;
		close(STDIN_FILENO);
	}
}

void	ft_free_heredoc(t_word *args, char ***envp,
	t_heredoc heredoc_vars, char *input)
{
	ft_free_all(envp, &args);
	free(heredoc_vars.filename);
	free(heredoc_vars.input);
	if (!input)
		ft_printf("\n");
	else
		free(input);
	close(heredoc_vars.fd);
}

void	ft_print_this_error(char ***envp)
{
	ft_print_error(5);
	ft_put_exitcode(envp, 1);
}

void	ft_bad_fd_error(char ***envp)
{
	ft_print_error(5);
	ft_put_exitcode(envp, 1);
}
