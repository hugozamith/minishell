#include "minishell.h"


char	**g_env = NULL;

const char *token_type_to_str(t_tokens type)
{
	switch (type)
	{
		case COMMAND:
			return "COMMAND";
		case ARGUMENT:
			return "ARGUMENT";
	//        case QUOTE:
	//           return "QUOTE";
		/* case VARIABLE:
			return "VARIABLE"; */
		case PIPE:
			return "PIPE";
		case REDIRECT_IN:
			return "REDIRECT_IN";
		case REDIRECT_OUT:
			return "REDIRECT_OUT";
		case END:
			return "END";
		default:
			return "UNKNOWN";
	}
}



int is_bt(char *word, t_word *args, char ***envp)
{
	if (!ft_strncmp(word, "echo", 5))
		return (bt_echo(args, 1));
	if (!ft_strncmp(word, "cd", 3))
		return (bt_cd(args, envp));
	if (!ft_strncmp(word, "pwd", 4))
		return (bt_pwd());
	if (!ft_strncmp(word, "export", 7))
		return (bt_export(args, envp));
	if (!ft_strncmp(word, "unset", 6))
		return (bt_unset(args, envp));
	if (!ft_strncmp(word, "env", 4))
		return (bt_env(*envp));
	if (!ft_strncmp(word, "exit", 5))
		return (ft_free_all(*envp, &args), bt_exit(args));
	return (1);
}

int main(int argc, char **argv, char **envp)
{
	char	**my_env;
	t_word	*next;
	char	*line = NULL;
	t_word	*args = NULL;
	t_word	*temp;

	argc = 0;
	argv = NULL;
	my_env = env_init(envp);
	if (!my_env)
		return (0);
	signal(SIGINT, ft_handlesignal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
		{
			ft_printf("exit\n");
			ft_free_all(my_env, &args);
			break ;
		}
		if (strlen(line) > 0)
			add_history(line);
		if (line)
		{
			lexer(line, &args);
			temp = args;
			while (temp)
			{
				printf("Token Type: %s, Value: %s\n",
					token_type_to_str(temp->type), temp->value);
				temp = temp->next;
			}
		}
		if (strcmp(line, "clear") == 0)
			ft_clear_screen();
		if (is_bt(args->value, args, &my_env))
		{
			ft_auto_execute(args);
		}
		free(line);
		while (args)
		{
			next = args->next;
			free(args->value);
			free(args);
			args = next;
		}
	}
	return (0);
}
