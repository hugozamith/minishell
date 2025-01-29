#include "minishell.h" 

int substring(const char *str, const char *sub)
{
	while (*str)
	{
		const char *s1 = str;
		const char *s2 = sub;

		while (*s1 && *s2 && (*s1 == *s2))
		{
			s1++;
			s2++;
		}

		if (*s2 == '\0')
		{
			return 1;
		}

		str++;
	}

	return 0;
}
