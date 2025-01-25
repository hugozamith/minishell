#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	while (*str1 == *str2 && *str1 && *str2)
	{
		++str1;
		++str2;
	}
	if (!*str1 || !*str2)
		return (0);
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

int	has_plus(char *str)
{
	if (str[ft_strlen(str) - 1] == '+')
		return (1);
	return (0);
}

int	ft_str_check(char *str)
{
	int	i;

	i = -1;
	if (has_plus(str))
		return (1);
	while (str[++i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]))
		{
			return (0);
		}
	}
	return (1);
}

int	ft_exportchecker(char **argv)
{
	if (!argv || !argv[0] || !ft_strcmp(argv[0], "1INVALID"))
	{
		ft_print_error(-1);
		ft_print_error(3);
		return (0);
	}
	if ((ft_strchr(argv[0], '-')))
	{
		ft_print_error(-1);
		ft_print_error(3);
		return (0);
	}
	if (!(argv[0][0] != '_' && ft_isalpha(argv[0][0])))
	{
		ft_print_error(-1);
		ft_print_error(3);
		return (0);
	}
	if (!ft_str_check(argv[0]))
	{
		ft_print_error(-1);
		ft_print_error(3);
		return (0);
	}
	return (1);
}

int	ft_put_exitcode(char ***envp, int nbr)
{
	static int	old_code_of_exit;

	if (nbr == -2)
		return (old_code_of_exit);
	(void) envp;
	old_code_of_exit = g_code_of_exit;
	g_code_of_exit = nbr;
	return (0);
}
