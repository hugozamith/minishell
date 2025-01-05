#include "minishell.h"

int	handle_export_input(t_word *args, char ***envp,
	t_word **dummy, char ***argv)
{
	if (!ft_strncmp(args->value, "export", 6)
		&& !ft_strncmp(args->next->value, "END", 3))
	{
		print_export(*envp);
		return (0);
	}
	*dummy = malloc(sizeof(t_word));
	if (!ft_strncmp(args->next->next->value, "END", 3))
		(*dummy)->value = expand_string(args->next, envp);
	else
		(*dummy)->value = ft_strjoin_free(ft_strdup(args->next->value),
				expand_string(args->next->next, envp));
	*argv = ft_split((*dummy)->value, '=');
	if (!ft_exportchecker(*argv))
	{
		ft_free_argvs(*argv);
		ft_put_exitcode(envp, 1);
		return (0);
	}
	return (-1);
}

int	updated_environment(char ***envp, char **argv, t_word *dummy)
{
	int	i;

	i = -1;
	while ((*envp)[++i])
	{
		if (!ft_strcmp((*envp)[i], argv[0]))
		{
			free((*envp)[i]);
			(*envp)[i] = dummy->value;
			ft_free_argvs(argv);
			free(dummy);
			return (0);
		}
	}
	*envp = ft_realloc(*envp, (i + 2));
	(*envp)[i] = dummy->value;
	(*envp)[i + 1] = NULL;
	ft_put_exitcode(envp, 0);
	ft_free_argvs(argv);
	free(dummy);
	return (0);
}

int	bt_export(t_word *args, char ***envp)
{
	char	**argv;
	t_word	*dummy;
	int		status;

	status = handle_export_input(args, envp, &dummy, &argv);
	if (status != -1)
		return (status);
	return (updated_environment(envp, argv, dummy));
}
