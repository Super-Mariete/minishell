# Executables and includes
NAME = unit-tests
DEBUG_NAME = dmsh
VAL_NAME = valmsh
LIBFT_A = ../../libft/libft.a
HEADERS = ../../includes/minishell.h\
		../../includes/msh_limits.h
# Files
SRC = test_lexing.c\
	../../signals/signals.c\
	../../variables/init_env.c\
	../../utils.c\
	../../readline/readline.c\
	../../readline/process_key.c\
	../../readline/process_arrows.c\
	../../readline/history.c\
	../../readline/readline_utils.c\
	../../parse/parse_utils.c\
	lexing_copy.c\
	mock.c
# 	../parse/ast_utils.c\
# 	../parse/parsing.c
# 	../exec/builtins.c
OBJ = $(SRC:.c=.o)
DEBUG_OBJ = $(SRC:.c=.debug.o)
VAL_OBJ = $(SRC:.c=.val.o)

# Flags
DEFINES = -DARG_MAX=20480 -DCMDMAX=10 -DBUF_MAX=2048 -DHIST_MAX=2048 -DCMDENV_MAX=2048 -DMAXNODES=1024
CFLAGS = -I. -I ../../libft -I ../../includes -Wall -Wextra -Werror $(DEFINES)# -pedantic-errors
DEBUGFLAGS = $(DEFINES) -g3 -fsanitize=address,undefined

# Original
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -s -C ../../libft

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
	@cppcheck --enable=warning,style,performance,portability \
         --language=c \
         --std=c11 \
         --template='{file}:{line},{severity},{id},{message}' \
         --suppress=missingIncludeSystem \
         --error-exitcode=1 \
		 --check-level=exhaustive\
		  $(SRC)

# Cleanup and remake
clean:
	@rm  -f $(OBJ) $(DEBUG_OBJ) $(VAL_OBJ)
	@make -s -C ../../libft clean

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME) $(VAL_NAME)
	@make -s -C ../../libft fclean

re: fclean all

.PHONY: all clean fclean re val debug check