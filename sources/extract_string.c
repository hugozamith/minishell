#include "minishell.h"

char *extract_string(char *input, int *len)
{
    char quote_type = *input;
    char *start = input;
    char *result = ft_strndup(start, 0);
    *len = 1;  // Considera a aspas de abertura
    input++;

    while (1)
    {
        while (*input && *input != quote_type)
        {
            (*len)++;
            input++;
        }

        if (*input == quote_type)
        {
            (*len)++;
            input++;
            char *tmp = result;
            result = ft_strjoin(tmp, ft_strndup(start, *len));
            free(tmp);
            break;
        }
        else
        {
            // String não fechada, pedir mais input
            //printf("> ");
			write(1, "> ", 2);
            char *more_input = get_next_line(STDIN_FILENO);
            char *tmp = result;
            result = ft_strjoin(tmp, "\n");
            result = ft_strjoin(result, more_input);
            free(tmp);
            free(more_input);
            input = result;  // Atualiza o ponteiro de input
        }
    }

    return result;
}
