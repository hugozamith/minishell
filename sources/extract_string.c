
/*#include "minishell.h"

void	quote_loop()
{
	ft_putstr_fd("unclosed quotes!!!!! naughty naughty!!\n", STDERR_FILENO);
}
char *extract_string(char *input, int *len)
{
    char quote_type = *input;
    char *result = ft_strndup("", 0);  // Initialize result as empty
    input++;  // Skip opening quote
    *len = 1;

    while (*input)
    {
        // Look for the closing quote within the current input
        if (*input == quote_type)
        {
            (*len)++;
            input++;
            break;  // Exit loop once the closing quote is found
        }

        // If we reach the end of input without finding a closing quote
        if (*(input + 1) == '\0')
        {
            // Trigger the error for unclosed quotes
            quote_loop();  // Print error message
            free(result);  // Clean up allocated memory
            return (NULL);  // Return NULL to indicate error
        }
        else
        {
            // Append current character to result string
            char temp[2] = {*input, '\0'};
            result = ft_strjoin_free(result, temp);
            (*len)++;
            input++;
        }
    }

    return (result);
}
*/
