# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hteixeir <hteixeir@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 10:09:06 by hteixeir          #+#    #+#              #
#    Updated: 2024/05/17 10:29:36 by hteixeir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#* NAME - Name of the file to be created *#
#* SRCS - List of source C files to be CCiled *#
#* OBJS - List of object files that will be generated during CCile *#
#* .PONY - tells the make, that these are not files to be addressed, *#
#* not to be confused with file beacause they are commands *#
#* %.o: %.c - its a rule to assing .o files as object files *#
#* and .c as C source files *#
NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC
RM = rm -rf
SRCS	=	ft_atoi.c\
			ft_bzero.c\
			ft_calloc.c\
			ft_isalnum.c\
			ft_isalpha.c\
			ft_isascii.c\
			ft_isdigit.c\
			ft_isprint.c\
			ft_itoa.c\
			ft_memchr.c\
			ft_memcmp.c\
			ft_memcpy.c\
			ft_memmove.c\
			ft_memset.c\
			ft_putchar_fd.c\
			ft_putendl_fd.c\
			ft_putnbr_fd.c\
			ft_putstr_fd.c\
			ft_split.c\
			ft_strchr.c\
			ft_strdup.c\
			ft_striteri.c\
			ft_strjoin.c\
			ft_strlcat.c\
			ft_strlcpy.c\
			ft_strlen.c\
			ft_strmapi.c\
			ft_strncmp.c\
			ft_strnstr.c\
			ft_strrchr.c\
			ft_strtrim.c\
			ft_substr.c\
			ft_tolower.c\
			ft_toupper.c\
			ft_lstadd_back.c\
			ft_lstadd_front.c\
			ft_lstclear.c\
			ft_lstdelone.c\
			ft_lstiter.c\
			ft_lstlast.c\
			ft_lstmap.c\
			ft_lstnew.c\
			ft_lstsize.c\
			get_next_line_bonus.c\
			get_next_line_utils_bonus.c\
	
OBJS    = $(SRCS:.c=.o)
all: $(NAME)
$(NAME): $(OBJS)
		ar rc $(NAME) $(OBJS)
clean:
	$(RM) -f $(OBJS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re