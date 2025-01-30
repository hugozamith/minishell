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

int	ft_free_heredoc(char **str_list, char **lines,
	t_heredoc heredoc_vars, char *input)
{
	if (input)
		ft_print_all_lines(str_list, heredoc_vars);
	else if (!input)
		ft_end_it(heredoc_vars, str_list);
	ft_free_all(heredoc_vars.env, &heredoc_vars.args);
	free(heredoc_vars.filename);
	free(heredoc_vars.input);
	if (!input)
		ft_printf("\n");
	else
		free(input);
	ft_free_it(str_list, lines, NULL);
	close(heredoc_vars.fd);
	return (open(".heredoc_tmp", O_RDONLY));
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
