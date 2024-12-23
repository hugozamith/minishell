#include "minishell.h"

static char	*ft_shelljoin(char *dir, char *command)
{
	int		dirlen;
	int		comlen;
	int		len;
	char	*result;

	dirlen = ft_strlen(dir);
	comlen = ft_strlen(command);
	len = dirlen + comlen;
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	ft_memcpy(result, dir, dirlen);
	result[dirlen] = '/';
	ft_memcpy(result + dirlen + 1, command, comlen);
	result[len + 1] = '\0';
	return (result);
}

static char	*ft_find_command(char *command)
{
	char	*path_env;
	char	*path_copy;
	char	*full_path;
	char	**dir;
	int		i;

	i = -1;
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	dir = ft_split(path_copy, ':');
	while (dir[++i])
	{
		full_path = ft_shelljoin(dir[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (ft_free_argvs(dir), full_path);
		}
		free(full_path);
	}
	ft_free_argvs(dir);
	free(path_copy);
	return (NULL);
}

char	*ft_args_to_line(t_word *args)
{
	char	*result;
	char	*old_str;
	//char	*expand_str;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (args->type == COMMAND || args->type == ARGUMENT)
	{
		old_str = result;
		//expand_str = expand_string(args);
		result = ft_strjoin(result, args->value);
		free(old_str);
		old_str = result;
		result = ft_strjoin(result, " ");
		free(old_str);
		//free(expand_str);
		args = args->next;
	}
	return (result);
}


static int	ft_exec_input(char *input, t_word **orgs, char ***env)
{
	char	*command_path;
	char	**args;
    int     fds[2];
	int		status;
	int		pid;
	int		i = 0;
	//t_word	*arg;

    fds[0] = dup(STDIN_FILENO);
	//ft_printf("dup input\n");
    fds[1] = dup(STDOUT_FILENO);
	//ft_printf("dup output\n");
	status = 0;
	//handle_redirections(orgs);
	i = handle_redirections(*orgs, env);

	if (i == -1 || i == -2)
	{
		//ft_printf("GOT PROBLEMS\n");
		if (handle_redirections(*orgs, env) == -2)
			ft_put_exitcode(env, 2);
		else
			ft_put_exitcode(env, 1);
		//ft_printf("BEFORE FREE\n");
		/* free(orgs->value);
		free(orgs); */
		//ft_free_args(*orgs);
		reset_fd(fds[0], fds[1]);
		//ft_printf("AFTER FREE\n");
		//ft_free_args(orgs);
		return (1);
	}
	if (has_redir((*orgs)->next))
		(*orgs)->next = rm_redir_node((*orgs)->next);
    if (i)
    {
		if (i == 69)
		{
			ft_put_exitcode(env, 2);
			reset_fd(fds[0], fds[1]);
        	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
			return (0);
		}
        ft_put_exitcode(env, 1);
		reset_fd(fds[0], fds[1]);
        return (0);
    }
	input = ft_args_to_line(*orgs);
	args = ft_split(input, ' ');
	free(input);
	if (!ft_strchr(args[0], '/'))
		command_path = ft_find_command(args[0]);
	else
		command_path = ft_strdup(args[0]);
		//command_path = args[0];
	//ft_printf("HERE\n\n");
	//printf("args[0]: %s\n", args[0]);
	//printf("args[1]: %s\n", args[1]);
	//ft_printf("SECOND\n");
	pid = fork();
	if (pid == 0)// Child process
	{
		//printf("input: %s \n", command_path);
		//ft_printf("FOUR\n VALUE: %s\n Value: %s\n Value: %s\n\n", args[0], args[1], args[2]);
		//ft_printf("THIRD\n");
		if (execve(command_path, args, *env) == -1)
		{
			//ft_printf("GIMME AN ERROR!\n");
			ft_print_error(0);
			//ft_printf_fd(STDERR_FILENO," command not found\n");
			ft_free_argvs(args); // todo 
			free(command_path);
			//free(input);
			//ft_free_all(env, &arg);
			//ft_put_exitcode(env, 1);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0) // Parent process
	{	
		//ft_printf("FOURTH\n");
		//wait(&status); // Wait for the child process to finish
		if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return(ft_put_exitcode(env, 1), 1) ; // Return a failure exit code
        }
		if (WIFEXITED(status)) {
			//ft_print_error(-1);
			//int exit_code = WEXITSTATUS(status);
			//ft_free_args(orgs);
			//orgs = NULL;
			//ft_printf("VALUE %d\n", WEXITSTATUS(status));
			if (WEXITSTATUS(status) == 2)
				ft_put_exitcode(env, 2);
			else if (WEXITSTATUS(status) == 0)
				ft_put_exitcode(env, 0);
			else if (WEXITSTATUS(status) == 1)
			{
				ft_put_exitcode(env, 127);
				return(1) ;
			}
			/* else
				ft_put_exitcode(env, 127); */
			//ft_put_exitcode(env, WEXITSTATUS(status));
			//status = 101;
			//return exit_code;
    		 // Return the exit code of the child process
        } else {
            return(ft_put_exitcode(env, 1), 1) ; // Return a failure exit code if the child didn't exit normally
        }
	}	//free(input);
	/* else
		ft_put_exitcode(env, 1); */
	//ft_printf("FIFTH\n");
	ft_free_argvs(args);
	free(command_path);
	reset_fd(fds[0], fds[1]);
	/* if (status != 101)
		ft_put_exitcode(env, 1); */
	//ft_put_exitcode(env, 1); 
	//ft_printf("VALUE: %d\n", status);
	//ft_put_exitcode(env, status);
	return (0);
}


void expand_args(t_word *args, char ***envp)
{
	t_word *temp = args;

	while (temp)
	{
		if (temp->type == ARGUMENT)
		{
			//printf("temp->value: %s\n", temp->value);	
			temp->value = expand_string(temp, envp);
			//printf("temp->value: %s\n", temp->value);
		}
		temp = temp->next;
	}
}

int	ft_auto_execute(t_word *args, char ***envp)
{
	char *input;

	//printf("args->value: %s\n", args->next->value);
	expand_args(args, envp);
	//printf("args->value: %s\n", args->next->value);
	input = ft_args_to_line(args);
	//ft_printf("AFTER: %s\n", input);
	return(ft_exec_input(input, &args, envp));
}
