#include "minishell.h"

int tokensrch(t_word *args, t_tokens token)
{
	while (args)
	{
		if (args->type == token)
			return (1);
		args = args->next;
	}
	return (0);
}

int bt_echo(t_word *args, int fd)
{
    int     newline;
    t_word  *current;
    char    *expanded;
    int     fds[2];

    // Save the original file descriptors
    fds[0] = dup(STDIN_FILENO);
    fds[1] = dup(STDOUT_FILENO);
    if (fds[0] == -1 || fds[1] == -1)
    {
        perror("dup failed");
        return (1);
    }

    newline = 1;
    current = args->next;

    // Handle any redirection
	while (tokensrch(args, REDIRECT_OUT) || tokensrch(args->prev, REDIRECT_OUT))
		args = args->next;
    if (handle_redirections(args) < 0)
    {
        return (1);
    }

    // If there's an error in redirection, return
    if (fd < 0)
    {
        reset_fd(fds[0], fds[1]);
        return (1);
    }

    // Check if we need to print without a newline
    if (current && ft_strncmp(current->value, "-n", ft_strlen(current->value)) == 0)
    {
        newline = 0;
        current = current->next;
    }

    // Print all arguments with expansion
    while (current && current->type == ARGUMENT)
    {
        expanded = expand_string(current);
        ft_putstr_fd(expanded, fd);
        free(expanded);

        if (current->next && current->next->type != END &&
            !(current->next->value[0] == '\'' || current->next->value[0] == '"') &&
            !(current->value[0] == '\'' || current->value[0] == '"'))
        {
            ft_putchar_fd(' ', fd);
        }

        current = current->next;
    }

    // Print newline if required
    if (newline)
	{
        ft_putchar_fd('\n', fd);
	}

    // Close file descriptor if it's not STDOUT
	if (fd != STDOUT_FILENO)
        close(fd);

    // Reset file descriptors to their original state
    reset_fd(fds[0], fds[1]);

    return (0);
}
