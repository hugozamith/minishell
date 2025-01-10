#include "minishell.h"

void	ft_handlesignal(int sigur)
{
	if (sigur == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_code_of_exit = 130;
	}
	else if (sigur == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_seg_fault(int sigur)
{
	if (sigur == SIGQUIT)
	{
		ft_printf("Quit (core dumped)\n");
		ft_put_exitcode(NULL, 128 + sigur);
	}
	if (sigur == SIGINT)
	{
		ft_printf("\n");
		ft_put_exitcode(NULL, 130);
	}
}
