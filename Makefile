LIBFT_PATH		=	./libraries/libft
LIBFT			=	$(LIBFT_PATH)/libft.a

FT_PRINTF_PATH	=	./libraries/ft_printf
PRINTF			=	$(FT_PRINTF_PATH)/libftprintf.a

READLINE_FLAGS	=	-lreadline
NCURSES_FLAGS	=	-lncurses  # Adicionamos ncurses aqui

SOURCES_DIR		=	sources
FT_PRINTF_FD_DIR=	$(SOURCES_DIR)/ft_printf_fd
SOURCES_FILES	=	$(wildcard $(SOURCES_DIR)/*.c) $(wildcard $(FT_PRINTF_FD_DIR)/*.c)

HEADER			=	$(SOURCES_DIR)/minishell.h

SOURCES			=	$(SOURCES_FILES)
OUTFILES_DIR	=	outfiles
OBJECTS			= 	$(patsubst %.c, $(OUTFILES_DIR)/%.o, $(notdir $(SOURCES_FILES)))

NAME			=	minishell

CC				=	clang
RM				=	rm -f
MKDIR			=	mkdir -p

CFLAGS			=	-Wall -Wextra -Werror -g

# Rule to compile .c files into .o files in the outfiles directory
$(OUTFILES_DIR)/%.o: $(SOURCES_DIR)/%.c
				$(MKDIR) $(OUTFILES_DIR)
				$(CC) $(CFLAGS) -c $< -o $@

all:			$(NAME)

$(NAME):		$(LIBFT) $(PRINTF) $(OBJECTS) $(HEADER)
				$(CC) $(CFLAGS) $(OBJECTS) $(PRINTF) $(LIBFT) $(READLINE_FLAGS) $(NCURSES_FLAGS) -o $(NAME)  # Flags de ncurses adicionadas

$(LIBFT):
				$(MAKE) -C $(LIBFT_PATH)

$(PRINTF):
				$(MAKE) -C $(FT_PRINTF_PATH)

clean:	
				$(MAKE) -C $(LIBFT_PATH) clean
				$(MAKE) -C $(FT_PRINTF_PATH) clean
				$(RM) $(OBJECTS)
				$(RM) -r $(OUTFILES_DIR)

fclean:			clean
				$(MAKE) -C $(LIBFT_PATH) fclean
				$(MAKE) -C $(FT_PRINTF_PATH) fclean
				$(RM) $(NAME)

valgrind: 
	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)

re:				fclean all

.PHONY:			all clean fclean re
