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
	minishell.c \
	ms_init.c \
	ms_update.c \
	ms_error_handling.c

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

.PHONY: all $(NAME) clean fclean re
