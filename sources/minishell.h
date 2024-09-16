/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hteixeir <hteixeir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:48:55 by hteixeir          #+#    #+#             */
/*   Updated: 2024/09/16 13:43:08 by hteixeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
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

typedef struct s_flags
{
	char *command;
	char *args;
}	t_flags;

typedef struct s_command
{
	char *command;
	char *args;
}	t_command;


#endif