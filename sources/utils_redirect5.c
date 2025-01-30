#include "minishell.h"

void	ft_end_it(t_heredoc heredoc_vars, char **str_list)
{
	write(0, "\n", 1);
	if (g_code_of_exit != 130)
		ft_print_all_lines(str_list, heredoc_vars);
}

int	ft_get_fd(t_heredoc heredoc_vars)
{
	if (!ft_strcmp(heredoc_vars.filename, "validnot"))
		return (-2);
	else if (heredoc_vars.filename
		&& ft_strcmp(heredoc_vars.filename, "/dev/tty") != 0)
		return (open(heredoc_vars.filename, O_CREAT
				| O_WRONLY | O_TRUNC, 0644));
	else
		return (STDOUT_FILENO);
}

char	*ft_get_filename(t_word *current)
{
	if (!current->prev)
		return (ft_strdup("validnot"));
	if (!ft_strcmp(current->next->next->value, "END"))
		return (ft_strdup("/dev/tty"));
	else if (ft_strcmp(current->next->next->value, ">")
		&& !ft_strcmp(current->next->next->next->value, "END"))
		return (NULL);
	else if (ft_strcmp(current->next->next->value, ">")
		&& ft_strcmp(current->next->next->next->value, "END"))
		return (merge_filename(current->next->next->next));
	else
		return (merge_filename(current->next->next->next));
}
