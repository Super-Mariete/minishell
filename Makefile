NAME = minishell
LIBFT_A = libft/libft.a
SRC = minishell.c\
	parsing/lexing.c\
	parsing/lexing1.c\
	parsing/utils.c\
	parsing/parsing.c\
	parsing/parsing1.c\
	parsing/expansion.c\
	parsing/heredoc.c\
	parsing/signals.c\
	parsing/shenv.c\
	parsing/wildcards.c\
	parsing/wc_utils.c\
	exec/ft_execute.c\
	exec/builtins/ft_echo.c\
	exec/builtins/ft_cd.c\
	exec/builtins/ft_pwd.c\
	exec/builtins/ft_env.c\
	exec/builtins/ft_getenv.c\
	exec/builtins/ft_setenv.c\
	exec/builtins/ft_unsetenv.c\
	exec/builtins/ft_export.c\
	exec/builtins/ft_unset.c\
	exec/builtins/ft_exit.c\
	exec/mac_stub.c\
	exec/aux_exec/apply_redirs.c\
	exec/aux_exec/has_pipe.c\
	exec/aux_exec/exec_builtin.c\

OBJ = $(SRC:.c=.o)
CFLAGS += -g3 -Ilibft #-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	@$(MAKE) bonus -C libft

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm  -f $(OBJ)
	@$(MAKE) clean -C libft

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C libft

re: fclean all
