#include "minishell.h"

int	g_code_of_exit = 0;

void	setup_signals(void)
{
	signal(SIGINT, ft_handlesignal);
	signal(SIGQUIT, SIG_IGN);
}

char	*read_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
		ft_printf("exit\n");
	else if (strlen(line) > 0)
		add_history(line);
	return (line);
}

void	execute_input(char *line, t_word **args, char ***my_env)
{
	if (!lexer(line, args))
	{
		if (has_pipe(*args))
			pipe_execution(*args, my_env);
		else if (is_bt((*args)->value, *args, my_env))
			ft_auto_execute(*args, my_env);
	}
}

void	cleanup(char *line, t_word **args, char ***my_env)
{
	free(line);
	ft_free_line_arguments(args);
	ft_free_all(my_env, args);
}

int	main(int argc, char **argv, char **envp)
{
	char	***my_env;
	t_word	*args;
	char	*line;

	(void)argc;
	(void)argv;
	my_env = env_init(envp);
	line = NULL;
	args = NULL;
	setup_signals();
	while (1)
	{
		line = read_input();
		if (line == NULL)
		{
			cleanup(NULL, &args, my_env);
			break ;
		}
		if (*line && *line != ' ')
			execute_input(line, &args, my_env);
		free(line);
		ft_free_line_arguments(&args);
	}
	return (0);
}
