# Executables and includes
NAME = unit-tests
DEBUG_NAME = dmsh
VAL_NAME = valmsh
LIBFT_A = ../libft/libft.a
HEADERS = ../includes/minishell.h\
		../includes/msh_limits.h\
		test.h

# Files
MAIN ?= ../main.c
SRC = $(MAIN)\
	../signals/signals.c\
	../variables/init_env.c\
	../utils.c
# 	../exec/builtins.c
OBJ = $(SRC:.c=.o)
DEBUG_OBJ = $(SRC:.c=.debug.o)
VAL_OBJ = $(SRC:.c=.val.o)

# Flags
CFLAGS = -I. -include test.h -I libft -I includes -Wall -Wextra -Werror# -pedantic-errors
DEBUGFLAGS = -g3 -fsanitize=address,undefined

# Original
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -C libft

%.o: %.c $(HEADERS)
	@cc $(CFLAGS) -c $< -o $@

# With fsanitize
%.debug.o: %.c $(HEADERS)
	@cc $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

debug: $(DEBUG_OBJ) $(LIBFT_A)
	@cc $(CFLAGS) $(DEBUGFLAGS) $(DEBUG_OBJ) $(LIBFT_A) -lreadline -o $(DEBUG_NAME)

# For valgrind
%.val.o: %.c $(HEADERS)
	@cc $(CFLAGS) -g3 -c $< -o $@

val: fclean $(VAL_OBJ) $(LIBFT_A)
	@cc $(CFLAGS) -g3 $(VAL_OBJ) $(LIBFT_A) -lreadline -o $(VAL_NAME)

# Static analisys
check:
	@cppcheck --inconclusive --enable=all --suppress=missingIncludeSystem --inconclusive --std=c11 --check-level=exhaustive $(SRC)

# Cleanup and remake
clean:
	@rm  -f $(OBJ) $(DEBUG_OBJ) $(VAL_OBJ)
	@make -C .. -C libft clean

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME) $(VAL_NAME)
	@make -C .. -C libft fclean

re: fclean all

.PHONY: all clean fclean re val debug check