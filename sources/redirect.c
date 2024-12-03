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

int handle_redirections(t_word *args, char ***envp)
{
    t_word *current;
    int fd;

    current = args;
    while (current)
    {
		//ft_printf("FIRST\n");
        if (current->type == REDIRECT_OUT)
        {
			//ft_printf("SECOND\n");
			//ft_printf("VALUE: %s\n", current->next->value);
            fd = open(current->next->value, O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC, 0644);
            //ft_printf("FD: %d\n", fd);
			if (fd < 0)
            {
                //perror("open");
				//ft_printf("SECOND.1\n");
				/* 
				
				 THE PROBLEM FOR TEST 102 IS HERE
				
				
				
				 */
				if (errno == EACCES)
				{
					//ft_printf("FOURTH.1\n");
                    ft_print_error(5);
					//ft_printf("HERE2\n");
					ft_put_exitcode(envp, 1);
				}
				ft_put_exitcode(envp, 127);
				ft_print_error(5);
                return (-1);  // Retorna erro se não puder abrir o arquivo
            }
			if (dup2(fd, STDOUT_FILENO) == -1)
            {
				//ft_printf("SECOND.2\n");
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
                return (/* ft_printf("HERE\n"), */ -1);
            }
			//ft_printf("FOURTH\n");
            close(fd);
			//ft_printf("SECOND-OUT\n");
        }
        else if (current->type == REDIRECT_APPEND)
        {
			//ft_printf("THIRD\n");
            fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
				//ft_printf("THIRD1\n");
                ft_print_error(5);
				ft_put_exitcode(envp, 1);
				//ft_printf("HERE1\n");
                return (-1);  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
				//ft_printf("THIRD2\n");
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == REDIRECT_IN)
        {
			//ft_printf("FOURTH\n");
            fd = open(current->next->value, O_RDONLY);
            if (fd < 0)
            {
				//ft_printf("FOURTH-IN\n");
                if (errno == EACCES)
				{
					//ft_printf("FOURTH.1\n");
                    ft_print_error(5);
					//ft_printf("HERE2\n");
					ft_put_exitcode(envp, 1);
				}
                else if (errno == ENOENT)
				{
					//ft_printf("FOURTH.2\n");
					//ft_printf("HELLO\n");
                    ft_print_error(4);
					ft_put_exitcode(envp, 1);
				}
                else
				{
					//ft_printf("FOURTH.3\n");
                    //perror("open");
					ft_print_error(5);
					ft_put_exitcode(envp, 1);
				}
                return (-1);  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
				//ft_printf("FOURTH.4\n");
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
                return (-1);
            }
            close(fd);
			//ft_printf("FOURTH-OUT\n");
        }
        else if (current->type == HEREDOC)
        {
			//ft_printf("FIFTH\n");
            fd = handle_heredoc(current->next->value);
            if (fd == -1)
                return (-1);  // Retorna erro se o heredoc falhar
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
                return (-1);
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
