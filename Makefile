NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
IFLAGS = -I $(LIBFT_DIR)/include
LFLAGS = -lreadline -L$(LIBFT_DIR) -lft
SOURCE_DIR = src
BUILT_INS_DIR = built_ins
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
	update.c

MS_FILENAMES += \
	$(BUILT_INS_DIR)/built_in.c \
	$(BUILT_INS_DIR)/cd.c \
	$(BUILT_INS_DIR)/echo.c \
	$(BUILT_INS_DIR)/env.c \
	$(BUILT_INS_DIR)/exit.c \
	$(BUILT_INS_DIR)/export.c \
	$(BUILT_INS_DIR)/pwd.c \
	$(BUILT_INS_DIR)/unset.c

MS_FILENAMES += \
	executor/execute_in_child.c \
	executor/execute_on_chain.c \
	executor/execute_no_chain.c \
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
