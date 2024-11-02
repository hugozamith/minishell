#include "minishell.h"

int bt_echo(t_word *args, int fd)
{
    int newline;
    t_word *current;
    char *expanded;

    newline = 1;
    current = args->next;

    fd = handle_redirections(args);
    if (fd < 0)
        return (1);

    if (current && ft_strncmp(current->value, "-n", ft_strlen(current->value)) == 0)
    {
        newline = 0;
        current = current->next;
    }

    while (current && current->type == ARGUMENT)
    {
        expanded = expand_string(current);
        ft_putstr_fd(expanded, fd);
        free(expanded);

        if (!(current->next && (*(current->next->value) == '\'' || *(current->next->value) == '"')))
            ft_putchar_fd(' ', fd);
        current = current->next;
    }

    if (newline)
        ft_putchar_fd('\n', fd);

    if (fd != STDOUT_FILENO)
        close(fd);

    return (0);
}

