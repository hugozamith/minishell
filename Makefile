LIBFT_PATH		=	./libraries/libft
LIBFT			=	$(LIBFT_PATH)/libft.a

FT_PRINTF_PATH	=	./libraries/ft_printf
PRINTF			=	$(FT_PRINTF_PATH)/libftprintf.a

READLINE_FLAGS	=	-lreadline

SOURCES_DIR		=	sources
SOURCES_FILES	=	$(wildcard $(SOURCES_DIR)/*.c)

HEADER			=	$(SOURCES_DIR)/minishell.h

SOURCES			=	$(SOURCES_FILES)
OBJECTS			= 	$(SOURCES:.c=.o)

NAME			=	minishell

CC				=	clang
RM				=	rm -f

CFLAGS			=	-Wall -Wextra -Werror -ggdb

.c.o:
				$(CC) $(CFLAGS) -c $< -o $@

all:			$(NAME)

$(NAME):		$(LIBFT) $(PRINTF) $(OBJECTS) $(HEADER)
				$(CC) $(CFLAGS) $(OBJECTS) $(PRINTF) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

$(LIBFT):
				$(MAKE) -C $(LIBFT_PATH)

$(PRINTF):
				$(MAKE) -C $(FT_PRINTF_PATH)

clean:	
				$(MAKE) -C $(LIBFT_PATH) clean
				$(MAKE) -C $(FT_PRINTF_PATH) clean
				$(RM) $(OBJECTS)

fclean:			clean
				$(MAKE) -C $(LIBFT_PATH) fclean
				$(MAKE) -C $(FT_PRINTF_PATH) fclean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
