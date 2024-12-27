#include "minishell.h"

int ft_is_bt(char *word)
{
	if (!ft_strncmp(word, "echo", 5))
		return (1);
	if (!ft_strncmp(word, "cd", 3))
		return (1);
	if (!ft_strncmp(word, "pwd", 4))
		return (1);
	if (!ft_strncmp(word, "export", 7))
		return (1);
	if (!ft_strncmp(word, "unset", 6))
		return (1);
	if (!ft_strncmp(word, "env", 4))
		return (1);
	return (0);
}

int is_bts_in_pipe(t_word *args)
{
	int		i;
	int		j;
	int 	pipe;
	int 	redirect;
	char	*str;

	i = 0;
	j = 0;
	pipe = 0;
	redirect = 0;
	str = args->value;
	while ((args || (pipe == 0 && redirect == 0)) && args->type != PIPE)
	{
		//ft_printf_fd(STDERR_FILENO, "VALUE: %s\n", args->value);
		/* if (!args->type)
			return (1); */
		if (args->type == PIPE)
		{
			//ft_printf_fd(STDERR_FILENO, "VALUE: %s\n", args->next->value);
			pipe = i;
		}
		if (args->type == REDIRECT_APPEND || args->type == REDIRECT_IN
				|| args->type == REDIRECT_OUT)
		{
			if (redirect)
				redirect = -1;
			else
				redirect = j;
		}
		i++;
		j++;
		args = args->next;
	}
	pipe = i;
	//ft_printf_fd(STDERR_FILENO, "PIPE: %d\tREDIRECT: %d\tSTR: %s\n", pipe, redirect, str);
	if (!ft_is_bt(str))
		return (0);
	else if (redirect < pipe && pipe && redirect && redirect != -1 )
	{	
		ft_print_error(-1);
		return (1);
	}
	return (0);
}

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

char *merge_filename(t_word *node)
{
    char *new_value;
	int i;

	new_value = ft_strdup("");
	while (node->type == ARGUMENT)
	{
		i = 0;
		if (node->value[i] == '"')
			i++;
		while (node->value[i] != '"' && node->value[i])
		{
			new_value = add_char(new_value, node->value[i]);
			i++;
		}
		if (node->O == 1)
		{
			//write(1, "aqui\n", 5);
			node = node->next;
		}
		else
			break;
	}

    return new_value;
}
int count_nodes(t_word *head)
{
    int count = 0;
    t_word *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

int handle_redirections(t_word *args, char ***envp)
{
    t_word *current;
	t_word *next;
    int fd;
	char	*filename;
	char	*first;

    current = args;
	//write(1, "a\n", 2);

	//printf("current->value: %s\n", token_type_to_str(current->next->type));
	/* ft_printf_fd(0, "HERE\n");
	ft_printf("HERE\n"); */
	first = args->value;
	while (current)
    {
		//ft_printf_fd(0, "VALUE: %s\n", current->value);
		//ft_printf("FIRrrST\n");
		//ft_printf("VALUE: %s\n", current->value);
        if (current->type == REDIRECT_OUT)
        {
			//write(1, "SECOND\n", 7);
			//ft_printf("HERE\n");
			filename = merge_filename(current->next);
			//ft_printf("HERE2\n");
			//printf("filename: %s\n", filename);
			//ft_printf("VALUE: %s\n", current->next->value);
			/* ft_printf_fd(0, "HERE2\n");
			ft_printf("HERE2\n"); */
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC , 0644);
            /* ft_printf_fd(0, "HERE3\n");
			ft_printf("HERE3\n"); */
			//ft_printf("HERE3\n");
			//ft_printf("FD: %d\n", fd);
			if (fd < 0)
            {
				//ft_printf_fd(0, "HERE4\n");
				//free(current->value);
				//ft_printf("SECOND.1");
				/* if (current->)					
					ft_printf_fd(0, "HERE4\n"); */
				free(filename);
				if (errno == EACCES)
				{
					//ft_printf("FOURTH.1\n");
                    ft_print_error(5);
					//ft_printf("HERE2\n");
					ft_put_exitcode(envp, 1);
				}
				/* free(filename);
				//ft_put_exitcode(envp, 0);
				ft_print_error(5); */
                return (-1);  // Retorna erro se não puder abrir o arquivo
            }
			//ft_printf("HERE4\n");
			//ft_printf_fd(0, "VALUE: %d\n", dup2(fd, STDOUT_FILENO));
			if (dup2(fd, STDOUT_FILENO) == -1)
            {
				//ft_printf("SECOND.2\n");
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
				free(filename);
                return (-1);
            }
			//ft_put_exitcode(envp, 3);
			/* ft_printf_fd(0, "HERE4\n");
			ft_printf("HERE4\n"); */
			//ft_printf("SECOND_END\n");
            close(fd);
			free(filename);
        }
        else if (current->type == REDIRECT_APPEND)
        {
			filename = merge_filename(current->next);
			//ft_printf("THIRD\n");
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
				//ft_printf("THIRD1\n");
                ft_print_error(5);
				ft_put_exitcode(envp, 1);
				//ft_printf("HERE1\n");
				free(filename);
                return (-1);  // Retorna erro se não puder abrir o arquivo
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
				//ft_printf("THIRD2\n");
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
				free(filename);
                return (-1);
            }
			free(filename);
            close(fd);
        }
        else if ((current->type == REDIRECT_IN) || (ft_strchr(current->value, '<') && ft_strcmp(first, "echo")))
        {
			if (!(count_nodes(current) > 2))
				return(69);
			filename = merge_filename(current->next);
			//ft_printf("FOURTH\n");
            fd = open(filename, O_RDONLY);
            if (fd < 0)
            {
				free(filename);
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
					//ft_printf_fd(STDERR_FILENO, "HERE %d\n", is_bts_in_pipe(args));
					//ft_printf_fd(STDERR_FILENO, "First Value: %s\t%d\n", current->prev->value, ft_strcmp(current->prev->value, "echo"));
					//ft_printf_fd(STDERR_FILENO, "Last Value: %s\t%d\n", current->next->next->value, ft_strncmp(current->next->next->value, "END", 3));
					if (!ft_strcmp(current->prev->value, "echo") && (!ft_strncmp(current->next->next->value, "END", 3) || !handle_redirections(current->next->next, envp)))
						return (-1);
					if ((has_pipe(args) && !is_bts_in_pipe(args))
						|| !has_pipe(args))
                    	ft_print_error(4);
					/* else
					{
						ft_printf_fd(STDERR_FILENO, "HEREHEHRHHE++EHRHER\n");
						ft_print_error(2);
					} */
					//ft_printf_fd(STDERR_FILENO, "\n");
					//ft_printf("Length: %d\n", ft_strlen(current->value));
					if (ft_strchr(current->value, '<') && ft_strlen(current->value) > 1)
						return (-2);
					else
						ft_put_exitcode(envp, 1);
					return (-1);
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
			free(filename);
			//ft_printf("FOURTH-OUT\n");
        }
        else if (current->type == HEREDOC)
        {
			filename = merge_filename(current->next);
			//ft_printf("FIFTH\n");
            fd = handle_heredoc(current->next->value);
            if (fd == -1)
                return (-1);  // Retorna erro se o heredoc falhar
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
				ft_put_exitcode(envp, 1);
                close(fd);
				free(filename);
                return (-1);
            }
            close(fd);
			free(filename);
        }
		//ft_printf_fd(0, "VALUE: %s\n", current->value);
        next = current->next;
		/* free(current->value);
		free(current); */
		current = next;
    }
	//ft_put_exitcode(envp, 3);
	//free(current);
	//current = current->prev;
	/* ft_printf_fd(0, "HERE5\n");
	ft_printf("HERE55\n"); */
	// while (current)
	// {
	// 	//ft_printf("VALUE: %s\n", (current)->value);
	// 	//ft_printf("THERES STILL ARG\n");
	// 	t_word *next = current;
	// 	current = next->next;
	// }
	//ft_printf_fd(0, "VALUE: %s\n", current->next->value);
	/* if (current->next)
		ft_printf("NOTHING HERE!"); */
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