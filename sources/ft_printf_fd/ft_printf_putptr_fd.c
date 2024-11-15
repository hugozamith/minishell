#include "../minishell.h"

static int	ft_put_ptr(unsigned long num, int fd)
{
	int	count;

	count = 0;
	if (num >= 16)
	{
		count += ft_put_ptr(num / 16, fd);
		count += ft_put_ptr(num % 16, fd);
	}
	else
	{
		if (num <= 9)
			count += ft_printfputchar_fd((num + '0'), fd);
		else
			count += ft_printfputchar_fd((num - 10 + 'a'), fd);
	}
	return (count);
}
/** @brief Writes the address of the pointer
 * 	@param ptr - pointer
 * 	@return Returns the size of the pointer
*/

int	ft_printfputptr_fd(unsigned long ptr, int fd)
{
	if (ptr == 0)
	{
		write(fd, "(nil)", 5);
		return (5);
	}
	else
	{
		write(fd, "0x", 2);
		return (ft_put_ptr(ptr, fd) + 2);
	}
	return (5);
}
