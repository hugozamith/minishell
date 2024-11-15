#include "../minishell.h"
/** @brief Writes a character on a specified file descriptor
 * 	@param c - character
 * 	@param fd - specified file descriptor
*/
int	ft_printfputchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

/*int	main(void)
{
	ft_putchar('t', 1);
	return (0);
}*/
