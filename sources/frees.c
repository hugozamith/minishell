#include "minishell.h"

void    ft_free_argvs(char **argv)
{
    int i;
    i = -1;
    while (argv[++i])
        free(argv[i]);
    free(argv);
}
void    ft_free_env(char ***env)
{
    int i;
    i = -1;
    //ft_printf("FREEING ENV\n");
    if(env[0] && *env[0])
    {
        //ft_printf("HELLO\n");
        //ft_printf("FIRST\n");
        while (env[0][++i])
        {
            free(env[0][i]);
        }
        //ft_printf("SECOND\n");
        //ft_printf("After: %s\n", env[i]);
        free(*env);
    }
    free(env);
    env = NULL;
    //ft_printf("After: %d\n", i);
}
void    ft_free_args(t_word *args)
{
    t_word *old_args;
    //ft_printf("FREEING ARGS\n");
    while(args->next)
    {
        old_args = args;
        args = args->next;
        free(old_args->value);
        free(old_args);
    }
    free(args->value);
    free(args);
    /* if (args)
        free(args); */
}
void    ft_free_all(char ***env, t_word **args)
{
    if (env && *env)
        ft_free_env(env);
    if (*args)
        ft_free_args(*args);
}






