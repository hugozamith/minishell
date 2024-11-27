#include "minishell.h"

int handle_heredoc(const char *delimiter)
{
    char *line;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2); // Prompt for heredoc input
        line = get_next_line(STDIN_FILENO); // Use uma função similar para obter a linha
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        if (line[ft_strlen(line) - 1] != '\n')
            write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    return pipefd[0]; // Retorna o descritor de leitura do pipe
}

int handle_redirections(t_word *args)
{
    t_word *current;
    int fd;

    current = args;
    while (current)
    {
        if (current->type == REDIRECT_OUT)
        {
            fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                return -1;  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        else if (current->type == REDIRECT_APPEND)
        {
            fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("open");
                return -1;  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        else if (current->type == REDIRECT_IN)
        {
            fd = open(current->next->value, O_RDONLY);
            if (fd < 0)
            {
                if (errno == EACCES)
                    fprintf(stderr, "minishell: permission denied: %s\n", current->next->value);
                else if (errno == ENOENT)
                    fprintf(stderr, "minishell: no such file or directory: %s\n", current->next->value);
                else
                    perror("open");
                return -1;  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        else if (current->type == HEREDOC)
        {
            fd = handle_heredoc(current->next->value);
            if (fd == -1)
                return -1;  // Retorna erro se o heredoc falhar
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        current = current->next;
    }

    return 0;
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
