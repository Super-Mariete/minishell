NAME = libft.a
SRC = ft_atoi.c ft_isascii.c ft_memcmp.c \
      ft_putendl_fd.c ft_strdup.c ft_strlen.c \
      ft_strtrim.c ft_bzero.c ft_isdigit.c \
      ft_memcpy.c ft_putnbr_fd.c ft_striteri.c\
      ft_strmapi.c ft_substr.c ft_calloc.c \
      ft_isprint.c ft_memmove.c ft_putstr_fd.c \
      ft_strjoin.c ft_strncmp.c ft_tolower.c \
      ft_isalnum.c ft_itoa.c ft_memset.c \
      ft_split.c ft_strlcat.c ft_strnstr.c \
      ft_toupper.c ft_isalpha.c ft_memchr.c \
      ft_putchar_fd.c ft_strchr.c ft_strlcpy.c \
      ft_strrchr.c ft_swap.c ft_rev_int_tab.c\
      ft_sort_int_tab.c ft_strcpy.c ft_strncpy.c\
      ft_putstr_non_printable.c ft_putchar.c\
      ft_strstr.c ft_convert_int.c ft_putnbr_base.c ft_longlen.c\
      ft_atoi_base.c ft_power.c ft_iterative_factorial.c\
      ft_fibonacci.c ft_convert_base.c ft_nbr_base_len.c\
      ft_neg.c ft_putnbr.c ft_free_d.c\
      ft_strnchr.c ft_esc_str.c ft_free_prev_d.c\
      ft_atol.c ft_abs.c ft_doubleptr_len.c\
      ft_find_in_matrix.c ft_isspace.c\
      ft_strndup.c ft_add_ptr.c ft_add_re_ptr.c\
      ft_strcmp.c

BONUS_SRC = ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstclear_bonus.c\
	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstnew_bonus.c\
	ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
CC = cc
OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
CFLAGS += -Wall -Wextra -Werror
AR = ar rsc

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
bonus: $(OBJ) $(BONUS_OBJ)
	$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)
clean:
	rm -f $(OBJ) $(BONUS_OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
