/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peferrei <peferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:48:55 by hteixeir          #+#    #+#             */
/*   Updated: 2024/12/29 17:11:43 by peferrei         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include "../libraries/libft/get_next_line_bonus.h"
# include "../libraries/libft/libft.h"
# include "../libraries/ft_printf/ft_printf.h"

extern char	**g_env;

typedef enum e_tokens
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_IN,
	HEREDOC,
	END
}	t_tokens;

typedef struct s_word
{
	t_tokens			type;
	char				*value;
	struct s_word		*next;
	struct s_word		*prev;
	int					O;
}	t_word;

typedef struct s_shelly
{
	int	exit_code;
	int	i;
	int	j;
	int	k;
	int	l;
}	t_shelly;

int			has_redir(t_word *args);
t_word		*rm_redir_node(t_word *args);
void		reset_fd(int saved_fd_in, int saved_fd_out);
void		pipe_execution(t_word *args, char ***rara);
int			bt_exit(t_word *args, char ***envp);
int			bt_pwd(void);
int			bt_cd(t_word *args, char ***envp);
int			bt_env(char **envp);
int			bt_unset(t_word *args, char ***envp);
int			is_bt(char *word, t_word *args, char ***envp);
int			split_count(char **split);
int			print_error(char *s1, char *s2, char *s3, char *message);
void		ft_free_split(char ***split);
void		print_env(char **envp);
char		***env_init(char **envp);
char		*add_char(char *str, char c);
void		delete_token_list(t_word **token_list);
char		*ft_strjoin_free(char *s1, char *s2);
char		*expand_string(t_word *input, char ***envp);
char		*extract_string(char *input, int *len);
char		*remove_quotations(char *str);
const char	*token_type_to_str(t_tokens type);
int			handle_redirections(t_word *args, char ***envp);
int			bt_echo(t_word *args, int fd, char ***envp);
int			ft_clear_screen(void);
int			lexer(char *input, t_word **token_list);
char		*ft_strndup(const char *s, int n);
void		ft_handlesignal(int sigur);
int			ft_auto_execute(t_word *args, char ***env);
int			bt_export(t_word *args, char ***envp);
int			ft_strcmp(const char *s1, const char *s2);
void		ft_free_all(char ***env, t_word **args);
void		ft_free_argvs(char **argv);
void		ft_free_args(t_word *args);
void		ft_free_env(char ***env);
char		*ft_getenv(char *var, char ***envp);
char		*ft_args_to_line(t_word *args);
int			ft_printf_fd(int fd, const char *s, ...);
int			ft_printfnumlen(int n);
int			ft_printfputchar_fd(char c, int fd);
int			ft_printfint_puthex_fd(unsigned int n, char c, int fd);
int			ft_printfputnbr_base_fd(unsigned int nbr, char *base, int fd);
int			ft_printfputnbr_fd(int nb, int fd);
int			ft_printfputptr_fd(unsigned long ptr, int fd);
int			ft_printfputstr_fd(char *str, int fd);
size_t		ft_printfstrlcpy(char *dst, const char *src, size_t size);
size_t		ft_printfstrlen(const char *str);
int			ft_printfstrncmp(const char *s1, const char *s2, size_t n);
void		ft_put_exitcode(char ***envp, int nbr);
char		*ft_getenv(char *var, char ***envp);
void		expand_args(t_word *args, char ***envp);
void		ft_print_error(int i);
int			has_pipe(t_word *args);
void		ft_redirect_free(t_word *current, char ***env);
void		ft_special_node_free(t_word **current);
int			count_pipes(t_word *args);
t_word		*get_next_command(t_word **args);
void		create_pipes(int pipe_count, int pipes[][2]);
void		handle_pipe_redirection(int i, int pipe_count, int pipes[][2]);
char		*command_to_str(t_word *command);
void		close_pipes(int pipe_count, int pipes[][2]);
void		ft_special_free(t_word *args);
void		ft_pipe_free(t_word *args);
int			ft_is_bt(char *word);
int			is_bts_in_pipe(t_word *args);
char		*merge_filename(t_word *node);
int			count_nodes(t_word *head);

#endif