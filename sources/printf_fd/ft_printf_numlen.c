#include "../minishell.h"
/** @brief Gives the number size
 * 	@param n - number
 *  @return Returns the size of the number
*/
int	ft_printfnumlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}
