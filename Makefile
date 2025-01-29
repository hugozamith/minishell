LIBFT_PATH		=	./libraries/libft
LIBFT			=	$(LIBFT_PATH)/libft.a

FT_PRINTF_PATH	=	./libraries/ft_printf
PRINTF			=	$(FT_PRINTF_PATH)/libftprintf.a

READLINE_FLAGS	=	-lreadline
NCURSES_FLAGS	=	-lncurses

SOURCES_DIR		=	sources
FT_PRINTF_FD_DIR=	$(SOURCES_DIR)/printf_fd

# Manually list source files
SOURCES_FILES	=	\
	$(SOURCES_DIR)/bt_cd.c \
	$(SOURCES_DIR)/bt_echo.c \
	$(SOURCES_DIR)/bt_env.c \
	$(SOURCES_DIR)/bt_exit.c \
	$(SOURCES_DIR)/bt_export.c \
	$(SOURCES_DIR)/bt_pwd.c \
	$(SOURCES_DIR)/bt_unset.c \
	$(SOURCES_DIR)/execute.c \
	$(SOURCES_DIR)/expand_string.c \
	$(SOURCES_DIR)/frees2.c \
	$(SOURCES_DIR)/frees.c \
	$(SOURCES_DIR)/lexer.c \
	$(SOURCES_DIR)/minishell.c \
	$(SOURCES_DIR)/pipes.c \
	$(SOURCES_DIR)/redirect.c \
	$(SOURCES_DIR)/signals.c \
	$(SOURCES_DIR)/utils.c \
	$(SOURCES_DIR)/utils_cd.c \
	$(SOURCES_DIR)/utils_echo2.c \
	$(SOURCES_DIR)/utils_echo.c \
	$(SOURCES_DIR)/utils_execute2.c \
	$(SOURCES_DIR)/utils_execute.c \
	$(SOURCES_DIR)/utils_expand_string.c \
	$(SOURCES_DIR)/utils_export3.c \
	$(SOURCES_DIR)/utils_export2.c \
	$(SOURCES_DIR)/utils_export.c \
	$(SOURCES_DIR)/utils_lexer2.c \
	$(SOURCES_DIR)/utils_lexer3.c \
	$(SOURCES_DIR)/utils_lexer4.c \
	$(SOURCES_DIR)/utils_lexer.c \
	$(SOURCES_DIR)/utils_minishell2.c \
	$(SOURCES_DIR)/utils_minishell.c \
	$(SOURCES_DIR)/utils_pipe2.c \
	$(SOURCES_DIR)/utils_pipe3.c \
	$(SOURCES_DIR)/utils_pipe.c \
	$(SOURCES_DIR)/utils_redirect3.c \
	$(SOURCES_DIR)/utils_redirect2.c \
	$(SOURCES_DIR)/utils_redirect.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_numlen.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_putchar_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_puthex_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_putnbr_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_putnbr_base_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_putptr_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_putstr_fd.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_strlcpy.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_strlen.c \
	$(FT_PRINTF_FD_DIR)/ft_printf_strncmp.c

HEADER			=	$(SOURCES_DIR)/minishell.h

OUTFILES_DIR	=	outfiles
OBJECTS			= 	$(patsubst %.c,$(OUTFILES_DIR)/%.o,$(notdir $(SOURCES_FILES)))

NAME			=	minishell

CC				=	clang
RM				=	rm -f

CFLAGS			=	-Wall -Wextra -Werror -g

all:			$(NAME)

# Ensure outfiles directory exists
$(OUTFILES_DIR):
				mkdir -p $(OUTFILES_DIR)

# Compilation rule (places all object files in `outfiles/`)
$(OUTFILES_DIR)/%.o: $(SOURCES_DIR)/%.c | $(OUTFILES_DIR)
				$(CC) $(CFLAGS) -c $< -o $@

$(OUTFILES_DIR)/%.o: $(FT_PRINTF_FD_DIR)/%.c | $(OUTFILES_DIR)
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(LIBFT) $(PRINTF) $(OBJECTS) $(HEADER)
				$(CC) $(CFLAGS) $(OBJECTS) $(PRINTF) $(LIBFT) $(READLINE_FLAGS) $(NCURSES_FLAGS) -o $(NAME)

$(LIBFT):
				$(MAKE) -C $(LIBFT_PATH)

$(PRINTF):
				$(MAKE) -C $(FT_PRINTF_PATH)

valgrind: 
	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	/usr/bin/valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)

clean:
				$(MAKE) -C $(LIBFT_PATH) clean
				$(MAKE) -C $(FT_PRINTF_PATH) clean
				$(RM) -r $(OUTFILES_DIR)

fclean:			clean
				$(MAKE) -C $(LIBFT_PATH) fclean
				$(MAKE) -C $(FT_PRINTF_PATH) fclean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re