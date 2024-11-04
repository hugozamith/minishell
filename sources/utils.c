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

/* int ft_clear_screen(void)
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
} */

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


/*char remove_quotations(char **str)
{
	char	*new;
	char	*old;
	int		i;
	int		j;

	i = 0;
	j = 0;
	old = *str;
	new = malloc(ft_strlen(old) + 1);
	if (new == NULL)
		return (1);
	while (old[i])
	{
		if (old[i] != '\'' && old[i] != '\"')
		{
			new[j] = old[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	free(old);
	*str = new;
	return (0);
}*/


char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*ret;
	char	*a;

	a = (char *)s1;
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ret = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	ft_memcpy(ret, a, len_s1);
	ft_strlcpy(ret + len_s1, s2, len_s2 + 1);
	free(s1);
	free(s2);
	return (ret);
}

void delete_token_list(t_word **token_list)
{
    t_word *current = *token_list;
    t_word *next_token;

    while (current)
    {
        next_token = current->next;
        free(current->value);  // Free the value string
        free(current);  // Free the current token structure
        current = next_token;
    }
    *token_list = NULL;  // Set the list pointer to NULL after deletion
}
char *add_char(char *str, char c)
{
	char *new_str;
	int len;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	ft_memcpy(new_str, str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}