#include "minishell.h"

int handle_redirections(t_word *args)
{
    t_word *current;
    int fd;

	//write(1, "handle_redirections\n", 20);
    current = args;
    while (current)
    {
		//printf("current->value: %s\n", current->value);
        if (current->type == REDIRECT_OUT)
        {
            //write(1, "REDIRECT_OUT\n", 13);
            fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                return (-1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd); // Close the file descriptor after redirecting
        }
        else if (current->type == REDIRECT_IN)
        {
            fd = open(current->next->value, O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                return (-1);
            }	
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd); // Close the file descriptor after redirecting
        }
        current = current->next;
    }

    return 0; // Success
}

void reset_fd(int saved_fd_in, int saved_fd_out)
{
    if (dup2(saved_fd_in, STDIN_FILENO) == -1)
    {
        perror("reset_fd: dup2(STDIN_FILENO) failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(saved_fd_out, STDOUT_FILENO) == -1)
    {
        perror("reset_fd: dup2(STDOUT_FILENO) failed");
        exit(EXIT_FAILURE);
    }
}