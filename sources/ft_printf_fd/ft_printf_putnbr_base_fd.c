#include "../minishell.h"

static int	print_base_nb(unsigned int nbr, char *base, int size, int fd)
{
	char			a;
	unsigned int	n;
	static int		len;

	len = 0;
	n = nbr;
	if (n >= (unsigned int)size)
		print_base_nb(n / size, base, size, fd);
	a = base[n % size];
	len += 1;
	write(fd, &a, 1);
	return (len);
}
/** @brief Writes a number into their unsigned decimal
 * 	@param nbr - number
 * 	@param base - the base in which it is to be formatted
 * 	@return Returns the len of the final number
*/

int	ft_printfputnbr_base_fd(unsigned int nbr, char *base, int fd)
{
	char	*str;
	int		s;

	s = 0;
	str = NULL;
	if (ft_strncmp(base, "ude", 3) == 0)
		str = "0123456789";
	while (str[s] != '\0')
		s++;
	return (print_base_nb(nbr, str, s, fd));
}
