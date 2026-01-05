NAME = minishell
LIBFT_A = libft/libft.a

SRC = main.c\
	signals/signals.c\
	variables/init_env.c\
	utils.c\
	readline/readline.c\
	readline/process_key.c\
	readline/process_arrows.c\
	readline/history.c\
	readline/readline_utils.c\
	parse/lexing.c\
	parse/parse_utils.c\
	parse/ast_utils.c\
	parse/parsing.c

# 	exec/builtins.c
OBJ = $(SRC:.c=.o)

CFLAGS = -I libft -I includes -g -Wall -Wextra -Werror -pedantic-errors

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -s -C libft

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm  -f $(OBJ) $(DEBUG_OBJ) $(VAL_OBJ)
	@make -s -C libft clean

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME) $(VAL_NAME)
	@make -s -C libft fclean

re: fclean all

.PHONY: all clean fclean re