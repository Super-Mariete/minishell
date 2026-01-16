# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gemini <gemini@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/16 00:00:00 by gemini            #+#    #+#              #
#    Updated: 2026/01/16 00:00:00 by gemini           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Targets ---
NAME_NORMAL = msh
NAME_SAN    = san_msh
NAME_VAL    = val_msh

# --- Directories ---
ROOT_DIR    = ../..
LIBFT_DIR   = $(ROOT_DIR)/libft
OBJ_DIR     = obj
OBJ_NORMAL  = $(OBJ_DIR)/normal
OBJ_SAN     = $(OBJ_DIR)/san
OBJ_VAL     = $(OBJ_DIR)/val

# --- Compiler & Flags ---
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3 -I$(ROOT_DIR) -I$(LIBFT_DIR)
SAN_FLAGS   = -fsanitize=address,undefined
LDFLAGS     = -L$(LIBFT_DIR) -lft -lreadline

# --- Sources ---
SRC         = $(ROOT_DIR)/minishell.c \
              $(ROOT_DIR)/parsing/lexing.c \
              $(ROOT_DIR)/parsing/lexing1.c \
              $(ROOT_DIR)/parsing/lexing2.c \
              $(ROOT_DIR)/parsing/utils.c \
              $(ROOT_DIR)/parsing/utils2.c \
              $(ROOT_DIR)/parsing/parsing.c \
              $(ROOT_DIR)/parsing/parsing1.c \
              $(ROOT_DIR)/parsing/parsing_utils.c \
              $(ROOT_DIR)/parsing/expansion.c \
              $(ROOT_DIR)/parsing/heredoc.c \
              $(ROOT_DIR)/parsing/heredoc2.c \
              $(ROOT_DIR)/parsing/signals.c \
              $(ROOT_DIR)/parsing/shenv.c \
              $(ROOT_DIR)/parsing/wildcards.c \
              $(ROOT_DIR)/parsing/wc_utils.c \
              $(ROOT_DIR)/exec/ft_execute.c \
              $(ROOT_DIR)/exec/builtins/ft_echo.c \
              $(ROOT_DIR)/exec/builtins/ft_cd.c \
              $(ROOT_DIR)/exec/builtins/ft_pwd.c \
              $(ROOT_DIR)/exec/builtins/ft_env.c \
              $(ROOT_DIR)/exec/builtins/ft_getenv.c \
              $(ROOT_DIR)/exec/builtins/ft_setenv.c \
              $(ROOT_DIR)/exec/builtins/ft_unsetenv.c \
              $(ROOT_DIR)/exec/builtins/ft_export.c \
              $(ROOT_DIR)/exec/builtins/ft_unset.c \
              $(ROOT_DIR)/exec/builtins/ft_exit.c \
              $(ROOT_DIR)/exec/mac_stub.c \
              $(ROOT_DIR)/exec/aux_exec/apply_redirs.c \
              $(ROOT_DIR)/exec/aux_exec/has_pipe.c \
              $(ROOT_DIR)/exec/aux_exec/exec_builtin.c \
              $(ROOT_DIR)/exec/exec_pipe.c \
              $(ROOT_DIR)/exec/exec_utils.c

# Mock files specific to this test
MOCK_SRC    = mock_files/mock_readline.c

# Combine sources
ALL_SRC     = $(SRC) $(MOCK_SRC)

# --- Objects ---
OBJS_NORMAL = $(addprefix $(OBJ_NORMAL)/, $(notdir $(ALL_SRC:.c=.o)))
OBJS_SAN    = $(addprefix $(OBJ_SAN)/, $(notdir $(ALL_SRC:.c=.o)))
OBJS_VAL    = $(addprefix $(OBJ_VAL)/, $(notdir $(ALL_SRC:.c=.o)))

# --- Rules ---

all: $(NAME_NORMAL) $(NAME_SAN) $(NAME_VAL)

# Normal Executable
$(NAME_NORMAL): $(LIBFT_DIR)/libft.a $(OBJS_NORMAL)
	$(CC) $(CFLAGS) $(OBJS_NORMAL) $(LDFLAGS) -o $@

# Sanitizer Executable
$(NAME_SAN): $(LIBFT_DIR)/libft.a $(OBJS_SAN)
	$(CC) $(CFLAGS) $(SAN_FLAGS) $(OBJS_SAN) $(LDFLAGS) -o $@

# Valgrind Executable
$(NAME_VAL): $(LIBFT_DIR)/libft.a $(OBJS_VAL)
	$(CC) $(CFLAGS) $(OBJS_VAL) $(LDFLAGS) -o $@

# Library Compilation
$(LIBFT_DIR)/libft.a:
	@make -C $(LIBFT_DIR)

# --- Object Compilation ---
vpath %.c $(dir $(ALL_SRC))

$(OBJ_NORMAL)/%.o: %.c
	@mkdir -p $(OBJ_NORMAL)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_SAN)/%.o: %.c
	@mkdir -p $(OBJ_SAN)
	$(CC) $(CFLAGS) $(SAN_FLAGS) -c $< -o $@

$(OBJ_VAL)/%.o: %.c
	@mkdir -p $(OBJ_VAL)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Clean Up ---
clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME_NORMAL) $(NAME_SAN) $(NAME_VAL)

re: fclean all

.PHONY: all clean fclean re
