NAME = minishell
LIBFT_A = libft/libft.a

SRC = main.c\
	signals/signals.c\
	variables/init_env.c\
	utils.c
# 	exec/builtins.c
OBJ = $(SRC:.c=.o)

CFLAGS = -I libft -I includes -Wall -Wextra -Werror -pedantic-errors

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -C libft

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm  -f $(OBJ) $(DEBUG_OBJ) $(VAL_OBJ)

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME) $(VAL_NAME)

re: fclean all

.PHONY: all clean fclean re