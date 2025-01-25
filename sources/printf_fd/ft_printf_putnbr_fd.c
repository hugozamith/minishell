#include "../minishell.h"
/** @brief Writes a character on a specified file descriptor
 * 	@param c - character
 * 	@param fd - specified file descriptor
*/
static void	ft_putchar(char c, int fd)
{
	write(fd, &c, 1);
}
/** @brief Writes a integer on a specified file descriptor
 * 	@param nb - number
 * 	@param fd - specified file descriptor
*/

int	ft_printfputnbr_fd(int nb, int fd)
{
	if (nb == -2147483648)
	{
		write(fd, "-", 1);
		ft_putnbr_fd(2, fd);
		ft_putnbr_fd(147483648, fd);
	}
	else if (nb < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
		ft_putnbr_fd(nb, fd);
		return (ft_printfnumlen(nb) + 1);
	}
	else if (nb < 10)
	{
		ft_putchar(nb + 48, fd);
	}
	else
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putchar(nb % 10 + 48, fd);
	}
	return (ft_printfnumlen(nb));
}
