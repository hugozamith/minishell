#include "../minishell.h"

/** @brief Checks the format and calls the corresponding function
 *  @param str - string
 * 	@param list_va - list of substrings to be formatted
 *  @return Returns the number of characters written        
*/
static int	ft_check(unsigned char str, va_list list_va, int fd)
{
	int	nbr;

	nbr = 0;
	if (str == 's')
		nbr += ft_printfputstr_fd(va_arg(list_va, char *), fd);
	else if (str == 'd' || str == 'i')
		nbr += ft_printfputnbr_fd(va_arg(list_va, int), fd);
	else if (str == 'c')
		nbr += ft_printfputchar_fd(va_arg(list_va, int), fd);
	else if (str == 'x')
		nbr += ft_printfint_puthex_fd(va_arg(list_va, unsigned long), 'x', fd);
	else if (str == 'X')
		nbr += ft_printfint_puthex_fd(va_arg(list_va, unsigned long), 'X', fd);
	else if (str == 'u')
		nbr += ft_printfputnbr_base_fd(va_arg(list_va, unsigned int),
				"ude", fd);
	else if (str == 'p')
		nbr += ft_printfputptr_fd(va_arg(list_va, unsigned long), fd);
	else if (str == '%')
	{
		write(fd, "%", 1);
		nbr += 1;
	}
	return (nbr);
}

/** @brief Displays a formatted string on the screen
 *  @param s - string
 * 	@param ... - the type of format for the string
 *  @return Returns the number of characters written
*/
int	ft_printf_fd(int fd, const char *s, ...)
{
	unsigned char	*str;
	va_list			valist;
	int				i;

	if (!s)
		return (0);
	i = 0;
	va_start(valist, s);
	str = (unsigned char *)s;
	while (*str)
	{
		if (*str == '%')
		{
			i += ft_check(*(str + 1), valist, fd);
			str += 2;
		}
		else
		{
			ft_putchar_fd(*str, fd);
			i += 1;
			str++;
		}
	}
	return (i);
}
