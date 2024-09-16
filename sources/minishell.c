#include "minishell.h"

int is_bt(char *word)
{
    if (!ft_strncmp(word, "echo", 10) || !ft_strncmp(word, "cd", 10)
		|| !ft_strncmp(word, "pwd", 10)
		|| !ft_strncmp(word, "export", 10)
		|| !ft_strncmp(word, "unset", 10)
		|| !ft_strncmp(word, "env", 10)
		|| !ft_strncmp(word, "exit", 10))
		return(42);
	return(0);
}

/*int parce_line(char *line)
{
	
}*/


int main(void)
{
    char *line = NULL;

    while (1)
	{
        line = readline("minishell$ ");
		if (is_bt(line))
			ft_printf("valid builtin!!\n");
        if (line == NULL) {
            printf("\n");
            break;
        }

        if (strlen(line) > 0) {
            add_history(line);
        }
        if (strcmp(line, "exit") == 0) {
			ft_printf("exit\n");
            free(line);
            break;	
        }

        printf("Comando recebido: %s\n", line);

        free(line);
    }

    return 0;
}
