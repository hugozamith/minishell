#include "minishell.h"

void print_env(char **envp)
{
    int i = 0;
	//ft_printf("SECOND\n");
	while (envp[i] != NULL)
        i++;
	//ft_printf("LINES: %d\t%s\n\n", i, envp[0]);
	i = -1;
    while (envp[++i] != NULL)
    {
		//ft_printf("Line: %d\t", i);
        printf("%s\n", envp[i]);
    }
	//ft_printf("IS THIS WRONG? %s\n", envp[i-1]);
}

char	**env_init(char **envp)
{
	int	i;
	char	**str;

	i = 0;
	while(envp[i])
		i++;
	str = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		str[i] = ft_strdup(envp[i]);
	str[i] = NULL;
	return(str);
}

int bt_env(char **envp)
{
	//ft_printf("FIRST\n");
	print_env(envp);
	return(1);
}
