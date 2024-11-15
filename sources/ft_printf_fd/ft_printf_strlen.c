#include "../minishell.h"
/** @brief Finds the length of a string
 *  @param str - string
 *  @return returns the length of the string
*/
size_t	ft_printfstrlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
