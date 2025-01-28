#include "minishell.h"

char	*ft_args_to_line(t_word *args)
{
	char		*result;
	t_tokens	first;
	t_word		*current;
	int			i;

	result = ft_strdup("");
	first = args->type;
	i = 0;
	current = args;
	if (first == REDIRECT_IN)
	{
		result = ft_strjoin_free(result, ft_strdup(current->value));
		result = ft_strjoin_free(result, ft_strdup(" "));
		current = current->next;
	}
	while (current->type == COMMAND || current->type == ARGUMENT)
	{
		result = ft_strjoin_free(result, ft_strdup(current->value));
		result = ft_strjoin_free(result, ft_strdup(" "));
		current = current->next;
	}
	return (result);
}

void	ft_exit_failure(char *command_path, char **args,
					char ***env, t_word *orgs)
{
	ft_print_error(0);
	if (args && *args)
		ft_free_argvs(args);
	if (command_path)
		free(command_path);
	ft_free_all(env, &orgs);
	exit(EXIT_FAILURE);
}

int	ft_handle_exit_status(int status, int exit_code, char ***env)
{
	exit_code = WEXITSTATUS(status);
	if (exit_code == 2)
		ft_put_exitcode(env, 2);
	else if (exit_code == 0)
		ft_put_exitcode(env, 0);
	else if (exit_code == 1)
		ft_put_exitcode(env, 127);
	else
		ft_put_exitcode(env, exit_code);
	if (exit_code != 0 && exit_code != 2)
		return (1);
	return (0);
}

char	*ft_special_args_to_line(t_word *args)
{
	char		*result;
	t_tokens	first;
	int			i;

	ft_reoganize_args(&args);
	result = ft_strdup("");
	first = args->type;
	i = 0;
	while (args && args->type != END)
	{
		result = ft_strjoin_free(result, ft_strdup(args->value));
		result = ft_strjoin_free(result, ft_strdup(" "));
		args = args->next;
	}
	return (result);
}

void	ft_reoganize_args(t_word **args)
{
	char		*first;
	char		*second;
	t_tokens	first_token;
	t_tokens	second_token;
	t_word		*dummy;

	first = (*args)->value;
	second = (*args)->next->value;
	first_token = (*args)->type;
	second_token = (*args)->next->type;
	dummy = *args;
	dummy = dummy->next->next;
	while (dummy && dummy->type != END)
	{
		dummy->prev->prev->value = dummy->value;
		dummy->prev->prev->type = dummy->type;
		dummy = dummy->next;
	}
	if (dummy->prev->prev)
	{
		dummy->prev->prev->value = first;
		dummy->prev->value = second;
		dummy->prev->prev->type = first_token;
		dummy->prev->type = second_token;
	}
}
