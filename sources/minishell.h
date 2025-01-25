/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peferrei <peferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:48:55 by hteixeir          #+#    #+#             */
/*   Updated: 2025/01/25 18:01:07 by peferrei         ###   ########.fr       */
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

extern int	g_code_of_exit;

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
	int					_o;
	int					_a;
}	t_word;

typedef struct s_heredoc
{
	char	*input;
	char	*first;
	char	*filename;
	int		fd;
}	t_heredoc;

typedef struct s_pipes
{
	t_word	*command;
	t_word	**retainer;
	int		pipe_count;
	int		status;
	int		*pid;
	int		i;
	int		**pipes;
}	t_pipes;

char		cornelius(char *dababy);
void		ft_bad_fd_error(char ***envp);
void		set_flag_o(t_word **token_list);
int			has_redir(t_word *args);
t_word		*rm_redir_node(t_word *args);
void		reset_fd(int saved_fd_in, int saved_fd_out);
void		pipe_execution(t_word *args, char ***rara);
int			get_flag(char *str, char c);
int			bt_exit(t_word *args, char ***envp);
int			bt_pwd(t_word *args);
int			bt_cd(t_word *args, char ***envp);
int			bt_env(t_word *args, char **envp);
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
int			handle_redirections(t_word *args, char ***envp,
				t_heredoc heredoc_vars);
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
int			ft_put_exitcode(char ***envp, int nbr);
char		*ft_getenv(char *var, char ***envp);
void		expand_args(t_word *args, char ***envp);
void		ft_print_error(int i);
int			has_pipe(t_word *args);
void		ft_redirect_free(t_word *current, char ***env);
void		ft_special_node_free(t_word **current);
int			count_pipes(t_word *args);
t_word		*get_next_command(t_word **args);
void		create_pipes(int pipe_count, int **pipes);
void		handle_pipe_redirection(int i, int pipe_count, int **pipes);
char		*command_to_str(t_word *command);
void		close_pipes(int pipe_count, int **pipes);
void		ft_special_free(t_word *args);
void		ft_pipe_free(t_word *args);
int			ft_is_bt(char *word);
int			is_bts_in_pipe(t_word *args);
char		*merge_filename(t_word *node);
int			count_nodes(t_word *head);
void		print_env(char **envp);
void		print_export(char **envp);
int			has_redir(t_word *args);
t_word		*rm_redir_node(t_word *args);
t_word		*remove_single_redirection_node(t_word *current);
int			ft_just_exit_code(t_word *args);
int			tokensrch(t_word *args, t_tokens token);
void		print_arguments(t_word *current, int fd, int newline, char ***envp);
int			ft_empty(t_word *current);
int			handle_redirection_logic(int i, int *fds, char ***envp);
int			init_fds(int *fds, char ***envp);
int			handle_exit_code(t_word *args, char ***envp);
int			ft_exportchecker(char **argv);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_realloc(char **envp, int size);
char		*ft_find_command(char *command, char ***env);
int			ft_handle_exit_status(int status, int exit_code, char ***env);
void		ft_exit_failure(char *command_path, char **args,
				char ***env, t_word *orgs);
char		*ft_args_to_line(t_word *args);
char		*prepare_command_and_args(t_word *orgs, char ***env, char ***args);
char		*ft_shelljoin(char *dir, char *command);
void		expand_args(t_word *args, char ***envp);
char		*ft_getenv(char *var, char ***envp);
char		*handle_shenanigans(char *result, char *current);
char		*expand_variable(char *str, char ***envp);
char		*handle_double_quotes(char *str, char ***envp);
char		*handle_single_quotes(char *str);
int			ft_handle_redirect_out(t_word *current, char ***envp);
int			ft_handle_redirect_append(t_word *current, char ***envp);
int			ft_handle_redirect_in(t_word *current, char ***envp, t_word *args);
void		ft_put_in_my_env(char ***envp, char *cwd);
int			handle_home_directory(char ***envp, char ***path);
void		ft_guarding_args(t_word *args, int i, int pipe_count);
void		ft_free_process_var(t_pipes *pipes_struct);
void		ft_init_pipes_struct(t_pipes **pipes_struct, t_word *args);
int			handle_pipe(char **input, t_word **token_list, int *pepi,
				t_tokens *prev_type);
int			handle_redirection(char **input, t_word **token_list,
				t_tokens *prev_type);
int			handle_single_quote(char **input, t_word **token_list,
				t_tokens *prev_type);
int			handle_double_quote(char **input, t_word **token_list,
				t_tokens *prev_type);
int			handle_word(char **input, t_word **token_list,
				t_tokens *prev_type, int *pepi);
void		ft_add_redir_token(char ***input, t_word **token_list,
				t_tokens type, char *value);
void		add_token(t_word **token_list, t_tokens type,
				char *value, int flag);
void		free_tokens(t_word **token_list);
void		bigproblem(void);
int			check_quotes_and_finalize(int single_quote_open,
				int double_quote_open, t_word **token_list);
char		*extract_word(char *input, int *len);
char		*extract_variable(char *input, int *len);
int			ft_belong_env(const char *path);
int			check_path(char *path, char ***envp);
void		ft_free_line_arguments(t_word **args);
int			ft_extras(char **word, t_word **args, char ***envp);
void		ft_seg_fault(int sigur);
void		setup_signals(void);
void		ft_reoganize_args(t_word **args);
char		*ft_special_args_to_line(t_word *args);
int			update_environment(char ***envp, char *path);
void		ft_bad_checker(char **argv, char *variable, char ***envp);
char		*ft_set_variable_value(t_word **dummy, char ***envp);
void		update_env(char ***envp, char *variable, char **argv);
int			handle_export_input(t_word *args, char ***envp);
char		*extract_word(char *input, int *len);
int			ft_sum_exit_code(t_word **args);
int			ft_only_redir(t_word *args);
int			has_plus(char *str);
void		free_2d_array(char **array);
char		*merge_filename(t_word *node);
void		ft_handle_heredoc_signal(int sigur);
void		ft_free_heredoc(t_word *args, char ***envp,
				t_heredoc heredoc_vars, char *input);
void		print_export_sort(char **envp);
int			handle_export_input(t_word *args, char ***envp);
char		*ft_only_arg(char *str);
void		ft_print_this_error(char ***envp);
void		ft_just_create(t_word *args);

#endif