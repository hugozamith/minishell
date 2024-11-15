#include "../minishell.h"
/** @brief Compares n characters between two strings
 * 	@param s1 - string one
 *  @param s2 - string two
 * 	@param n - number of characters to compare
 *  @return Returns the difference between 
 * 	the different characters of both strings.
*/
int	ft_printfstrncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char) s1[i] != (unsigned char) s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}
