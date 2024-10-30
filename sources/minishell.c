#include "minishell.h"


char	**g_env = NULL;

int has_pipe(t_word *args)
{
	while (args)
	{
		if (args->type == PIPE)
			return (1);
		args = args->next;
	}
	return (0);
}

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



int is_bt(char *word, t_word *args, char ***envp)
{
	if (!ft_strncmp(word, "echo", 5))
		return (bt_echo(args, 1));
	if (!ft_strncmp(word, "cd", 3))
		return (bt_cd(args));
	if (!ft_strncmp(word, "pwd", 4))
		return (bt_pwd());
	if (!ft_strncmp(word, "export", 7))
		return (bt_export(args, envp));
	if (!ft_strncmp(word, "unset", 6))
		return (bt_unset(args, envp));
	if (!ft_strncmp(word, "env", 4))
		return (bt_env(*envp));
	if (!ft_strncmp(word, "exit", 5))
		return (bt_exit(args));
	return (1);
}
int main(int argc, char **argv, char **envp)
{
    char ***my_env;
    argc = 0;
    argv = NULL;
    my_env = &envp;
    signal(SIGINT, ft_handlesignal);
    signal(SIGQUIT, SIG_IGN);
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
			// while (temp)
			// {
			// 	printf("Token Type: %s, Value: %s\n", token_type_to_str(temp->type), temp->value);
			// 	temp = temp->next;
			// }
			//printf("line 		: %s\n", line);
            if (has_pipe(args)) {
                pipe_execution(args, my_env);
            } else if (is_bt(args->value, args, my_env)) {
                ft_auto_execute(line);
            }
        }
        
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
