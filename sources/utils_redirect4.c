#include "minishell.h"

void	ft_print_all_lines(char **list, t_heredoc heredoc_vars)
{
	int	i;
	int	max;

	max = 0;
	while (list[max])
		max++;
	i = -1;
	if (heredoc_vars.fd != -2)
	{
		while (list[++i] != NULL)
		{
			ft_printf_fd(heredoc_vars.fd, "%s", list[i]);
			if (list[i] != NULL)
				ft_printf_fd(heredoc_vars.fd, "\n");
		}
	}
}

void	ft_free_it(char **str_list, char **lines, char *input)
{
	if (lines)
		free_2d_array(lines);
	if (str_list)
		free_2d_array(str_list);
	if (input)
		free(input);
}

char	**ft_new_list(char **str_list, int i, char *input)
{
	char	**new_list;

	new_list = ft_realloc(str_list, i + 1);
	new_list[i - 1] = ft_strndup(input, ft_strlen(input) - 1);
	new_list[i] = NULL;
	return (new_list);
}

void	ft_close_it(int fd)
{
	if (fd != -2)
		close(fd);
}

char	**ft_list_init(void)
{
	char	**str_list;

	str_list = malloc(sizeof(char *));
	str_list[0] = NULL;
	return (str_list);
}
