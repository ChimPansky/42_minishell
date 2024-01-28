NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
IFLAGS = -I $(LIBFT_DIR)/include
LFLAGS = -lreadline -L$(LIBFT_DIR) -lft
SOURCE_DIR = src
MS_HEADER = $(SOURCE_DIR)/$(NAME).h

include $(LIBFT_DIR)/colors.mk

MS_FILENAMES = \
	destroy.c \
	exit_error.c \
	expander.c \
	init.c \
	lexer.c \
	list_commands.c \
	list_tokens.c \
	list_variables.c \
	minishell.c \
	parser.c \
	scratches.c \
	signals.c \
	update.c

MS_FILENAMES += \
	built_ins/built_in.c \
	built_ins/cd.c \
	built_ins/echo.c \
	built_ins/env.c \
	built_ins/exit.c \
	built_ins/export.c \
	built_ins/pwd.c \
	built_ins/unset.c

MS_FILENAMES += \
	executor/execute_in_child.c \
	executor/execute_on_chain.c \
	executor/executor.c \
	executor/redirections.c

SRC = $(addprefix $(SOURCE_DIR)/,$(MS_FILENAMES))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)
	@echo "$(GREEN)Executable $(NAME) created!$(DEF_COLOR)"

$(LIBFT):
	@make -C $(LIBFT_DIR) all clean

%.o : %.c $(MS_HEADER)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re test
