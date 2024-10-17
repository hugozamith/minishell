
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hteixeir <hteixeir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:48:55 by hteixeir          #+#    #+#             */
/*   Updated: 2024/09/16 17:42:09 by hteixeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <curses.h>
# include <termcap.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../libraries/libft/get_next_line_bonus.h"
#include "../libraries/libft/libft.h"
#include "../libraries/ft_printf/ft_printf.h"

extern char	**g_env;


typedef enum e_tokens
{
	COMMAND,
	ARGUMENT,
	//OPERATOR,
	QUOTE,
	VARIABLE,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	END
}	t_tokens;


typedef struct s_word
{
	t_tokens			type;
	char				*value;
	struct s_word		*next;
}	t_word;

//
//typedef struct s_command
//{
//	char *command;
//	char *args;
//}	t_command;int	env_init(void)

int bt_exit(t_word *args);
int bt_pwd(void);
int bt_cd(t_word *args);
int bt_env(char **envp);
int bt_unset(t_word *args, char ***envp);
int is_bt(char *word, t_word *args, char ***envp);
int	split_count(char **split);
int	print_error(char *s1, char *s2, char *s3, char *message);
void	ft_free_split(char ***split);
void print_env(char **envp);
char	**env_init(char **envp);
int		ft_clear_screen(void);
void	lexer(char *input, t_word **token_list);
char	*ft_strndup(const char *s, int n);
void ft_handlesignal(int sigur);
void ft_auto_execute(char *input);
int	bt_export(t_word *args, char ***envp);
int	ft_strcmp(const char *s1, const char *s2);

#endif