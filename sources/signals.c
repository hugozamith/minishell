#include "minishell.h"

void ft_handlesignal(int sigur)
{
	if (sigur == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();  // Tell readline we're on a new line
    	rl_replace_line("", 0);  // Clear the current input line
    	rl_redisplay();  // Redisplay the prompt
	}
	else if (sigur == SIGQUIT)
	{
		rl_replace_line("", 0);  // Clear the current input line
		rl_on_new_line();  // Tell readline we're on a new line
    	rl_redisplay();  // Redisplay the prompt
	}
	else 
	{
		//ft_printf("\n");
		rl_on_new_line();  // Tell readline we're on a new line
    	rl_replace_line("", 0);  // Clear the current input line
    	rl_redisplay();  // Redisplay the prompt
	}
}
