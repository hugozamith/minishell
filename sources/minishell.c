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
        case VARIABLE:
            return "VARIABLE";
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



int is_bt(char *word, t_word *args)
{
//	if (!ft_strncmp(word, "echo", 5))
//		return (bt_echo(args));
	if (!ft_strncmp(word, "cd", 3))
		return (bt_cd(args));
	if (!ft_strncmp(word, "pwd", 4))
		return (bt_pwd());
//	if (!ft_strncmp(word, "export", 7))
//		return (bt_export(args));
//	if (!ft_strncmp(word, "unset", 6))
//		return (bt_unset(args));
	if (!ft_strncmp(word, "env", 4))
		return (bt_env());
	if (!ft_strncmp(word, "exit", 5))
		return (bt_exit(args));
	return (1);
}


/*int parce_line(char *line)
{
	
}*/

int main(void)
{
	signal(SIGINT, ft_handlesignal);
	signal(SIGQUIT, SIG_IGN);
	env_init();
    char *line = NULL;
    t_word *args = NULL;
    while (1)
	{
        line = readline("minishell$ ");
		if (line == NULL) {
           	ft_printf("exit\n");
            break;
        }
        if (strlen(line) > 0)
            add_history(line);
		if (line)
		{
			lexer(line, &args);
			t_word *temp = args;
			while (temp)
			{
				//printf("Token Type: %s, Value: %s\n", token_type_to_str(temp->type), temp->value);
				temp = temp->next;
			}

		}

		if (is_bt(args->value, args))
		{
			ft_auto_execute(line);	
			//printf("HEHEHEHEHEHE");
		}
//		if (strcmp(line, "env") == 0)
//		{
//			env_init();
//			print_env();
//		}
		if (strcmp(line, "clear") == 0)
			ft_clear_screen();
        //printf("Comando recebido: %s\n", line);
        free(line);
		while (args)
		{
			t_word *next = args->next;
			free(args->value);
			free(args);
			args = next;
		}
    }


    return 0;
}
