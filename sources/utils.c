#include "minishell.h"

int	split_count(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
		i++;
	return (i);
}

int	print_error(char *s1, char *s2, char *s3, char *message)
{
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
	{
		if (s1)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(s2, 2);
	}
	if (s3)
	{
		if (s1 || s2)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	if (message)
	{
		if (s1 || s2 || s3)
			ft_putstr_fd(": ", 2);
		ft_putstr_fd(message, 2);
	}
	ft_putchar_fd('\n', 2);
	return (-1);
}

void	ft_free_split(char ***split)
{
	int	i;

	i = 0;
	while ((*split)[i] != NULL)
	{
		free((*split)[i]);
		(*split)[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

int ft_clear_screen(void)
{
    char *term_type;
    char buffer[2048]; // Buffer para armazenar o conteúdo do termcap
    char *clear_cmd;

    // Obtém o tipo de terminal
    term_type = getenv("TERM");
    if (term_type == NULL)
    {
        perror("TERM environment variable not set");
        return (1);
    }

    // Inicializa a base de dados do termcap para o tipo de terminal
    if (tgetent(buffer, term_type) < 0)
    {
        perror("Could not access the termcap database");
        return (1);
    }

    // Obtém a sequência de controle para limpar a tela
    clear_cmd = tgetstr("cl", NULL);
    if (clear_cmd == NULL)
    {
        perror("Could not get clear screen command");
        return (1);
    }

    // Envia a sequência para limpar a tela
    tputs(clear_cmd, 1, putchar);
	printf("\033[3J");
    return (0);
}

char	*ft_strndup(const char *s, int n)
{
	size_t	size;
	char	*str;
	char	*ret;

	size = ft_strlen(s) + 1;
	if ((int)size > n)
		size = n + 1;
	str = malloc(size);
	if (NULL == str)
		return (NULL);
	ret = str;
	while (*s && n--)
		*str++ = *s++;
	*str = '\0';
	return (ret);
}