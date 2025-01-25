#include "../minishell.h"
/** @brief Writes a string on a specified file descriptor
 * 	@param str - string
 * 	@param fd - specified file descriptor
*/
int	ft_printfputstr_fd(char *str, int fd)
{
	int		i;

	if (str == NULL)
		return (write(fd, "(null)", 6));
	if (str)
	{
		i = 0;
		while (str[i] != '\0')
		{
			write(fd, &str[i], 1);
			i++;
		}
	}
	return (ft_printfstrlen(str));
}
