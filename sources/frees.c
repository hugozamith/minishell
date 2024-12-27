#include "minishell.h"

void    ft_free_argvs(char **argv)
{
    int i;
    i = -1;
    while (argv[++i])
        free(argv[i]);
    free(argv);
}
void	ft_free_env(char ***env)
{
	int	i;

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

	//ft_printf("THIRD\n");
	//ft_printf("FREEING ARGS\n");
	/* if (args->prev)
		while (args->prev)
			args = args->prev; */
	while(args->next)
	{
		//ft_printf("VALUE: %s\n", old_args->value);
		old_args = args;
		args = (args)->next;
		free(old_args->value);
		free(old_args);
		/* if (args == NULL)
			break ; */
	}
	//ft_printf("FOURTH\n");
	free(args->value);
	free(args);
	args = NULL;
	/* if (args)
		free(args); */
}

void	ft_free_all(char ***env, t_word **args)
{
	//ft_printf("VALUE: %s\n", (*args)->value);
	//ft_printf("VALUE: %s\n", args[1]->value);
	//ft_printf("FIRST\n");
	//ft_printf_fd(0, "Got in\n");
	if (env && *env)
		ft_free_env(env);
	//ft_printf("SECOND\n");
	if (args && *args)
	{
		//ft_printf_fd(0, "HEREEEE\n");
	 	ft_free_args(*args);
	}
	/* else if (!args)
		ft_printf("HEERE\n");
	else if (!*args)
		ft_printf("HEERE1\n"); */
}

void ft_redirect_free(t_word *current, char ***env)
{
	t_word *dummy;
	t_word *current_dummy;
	int	i;

	(void) current;
	//dummy = NULL;
	dummy = current;
	i = 0;
	/* if (dummy->type == END)
	{ */
		/* while (dummy->type != REDIRECT_OUT && dummy->type != REDIRECT_APPEND && dummy->type != REDIRECT_IN)
			dummy = dummy->prev; */
	/* }
	else
	{ */
		/* dummy = current;
		current_dummy = dummy;
		while (current_dummy->type != PIPE && current_dummy->type != END)
		{
			if (current_dummy->type == REDIRECT_OUT)
				dummy = current_dummy;
			current_dummy = current_dummy->next;
		} */
	//}
	//ft_printf("IN HERE3.95\n");
	ft_put_exitcode(env, 5);
	current_dummy = current;
	while (current_dummy->prev)
	{
		if (current_dummy->type == REDIRECT_OUT || current_dummy->type == REDIRECT_APPEND || current_dummy->type == REDIRECT_IN)
			dummy = current_dummy;
		current_dummy = current_dummy->prev;
	}
	/* while (dummy->type != PIPE && dummy->type != END)
	{
		dummy = dummy->next;
	} */
	/* if (dummy->type != END && dummy->next->type != END)
	{
		ft_printf_fd(0, "VALUE: %s\n", dummy->value);
		dummy = dummy->next->next;
	} */
	/* if (current->prev->prev->type == REDIRECT_OUT)
	{ */
		//ft_printf_fd(0, "VALUE: %s\n", current->next->value);
		//ft_put_exitcode(envp, 0);
	//ft_printf_fd(0, "VALUE after: %s\n", dummy->next->value);
	//ft_printf("IN HERE3.99\n");
	while (i <= 1)
	{
		t_word *next = dummy->next;
		free(dummy->value);
		free(dummy);
		dummy = next;
		i++;
	}
	//} 
}