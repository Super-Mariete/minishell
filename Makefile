NAME = minishell
DEBUG_NAME = dmsh
VAL_NAME = valmsh
LIBFT_A = libft/libft.a
SRC = main.c\
	signals/signals.c
# 	exec/builtins.c\
# 	exec/exec.c\
# 	exec/exec_pipe.c\
# 	exec/ft_cd.c\
# 	exec/ft_export.c\
# 	exec/ft_unset.c\

OBJ = $(SRC:.c=.o)
DEBUG_OBJ = $(SRC:.c=.debug.o)
VAL_OBJ = $(SRC:.c=.val.o)
CFLAGS = -I libft -I includes -Wall -Wextra -Werror -pedantic-errors
DEBUGFLAGS = -fsanitize=address,undefined

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -C libft

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

%.debug.o: %.c
	@cc $(CFLAGS) $(DEBUGFLAGS) -g -c $< -o $@

%.val.o: %.c
	@cc $(CFLAGS) -g -c $< -o $@

debug: $(DEBUG_OBJ) $(LIBFT_A)
	@cc $(CFLAGS) $(DEBUGFLAGS) $(DEBUG_OBJ) $(LIBFT_A) -lreadline -o $(DEBUG_NAME)

val: fclean $(VAL_OBJ) $(LIBFT_A)
	@cc $(CFLAGS) $(VAL_OBJ) $(LIBFT_A) -lreadline -o $(VAL_NAME)
	valgrind -s --track-origins=yes ./$(VAL_NAME)

check:
	@cppcheck --inconclusive --enable=all --suppress=missingIncludeSystem --inconclusive --std=c11 --check-level=exhaustive $(SRC)

clean:
	@rm  -f $(OBJ) $(DEBUG_OBJ) $(VAL_OBJ)

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME) $(VAL_NAME)

re: fclean all

.PHONY: all clean fclean re val debug check