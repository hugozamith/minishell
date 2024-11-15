#include "../minishell.h"
/** @brief Writes a number into their hexadecimal form
 * 	@param n - number
 * 	@param c - the base in which it is to be formatted
 * 	@return Returns the size of it's final form
*/
int	ft_printfint_puthex_fd(unsigned int n, char c, int fd)
{
	int		count;
	char	*hex;

	count = 0;
	if (c == 'x')
		hex = "0123456789abcdef";
	if (c == 'X')
		hex = "0123456789ABCDEF";
	if (n >= 16)
	{
		count += ft_printfint_puthex_fd((n / 16), c, fd);
		count += ft_printfint_puthex_fd((n % 16), c, fd);
	}
	else
		count += ft_printfputchar_fd(*(hex + n), fd);
	return (count);
}
