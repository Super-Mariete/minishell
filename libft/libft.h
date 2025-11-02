/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:27:40 by rafael-m          #+#    #+#             */
/*   Updated: 2025/10/31 19:37:11 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <limits.h>

# define ESC_CHARS "\\\'\"\?abfnrtv"

int			ft_atoi(const char *str);
void		*ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
char		*ft_itoa(int n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char *str, char charset);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *src);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlen(const char *str);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int			ft_strncmp(char *s1, char *s2, unsigned int n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t n);
char		*ft_strrchr(const char *s, int c);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_tolower(int c);
int			ft_toupper(int c);
void		ft_swap(int *a, int *b);
void		ft_rev_int_tab(int *tab, size_t size);
void		ft_sort_int_tab(int *tab, size_t size);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}			t_list;

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
char		*ft_strcpy(char *dest, char *src);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void		ft_putstr_non_printable(char *str);
void		ft_putchar(char c);
char		*ft_strstr(const char *str, const char *to_find);
size_t		ft_longlen(long n);
void		ft_putnbr_base(long nbr, char *base);
int			ft_atoi_base(char *str, char *base);
int			ft_power(int base, int power);
int			ft_iterative_factorial(int nb);
int			ft_fibonacci(int index);
char		*ft_convert_base(char *nbr, char *base_from, char *base_to);
char		*ft_convert_int(long nbr, char *base);
int			ft_nbr_base_len(long nbr, size_t base_len);
int			ft_neg(long n);
void		ft_rev_char_tab(char *tab);
void		ft_putnbr(int n);
char		*ft_strndup(const char *src, size_t n);
void		ft_free_d(char **ptr);
char		*ft_strnchr(const char *s, int c, unsigned int n);
char		*ft_esc_str(char *s, char *esc_char, size_t n);
void		ft_free_prev_d(char **s, int pos);
long		ft_atol(const char *s);
int			ft_doubleptr_len(void **ptr);
int			ft_find_in_matrix(char **m, char *to_find, int len);
size_t		ft_abs(int n);
int			ft_isspace(int c);
// char		*ft_trim_spaces(char *s);
void		**ft_add_ptr(void **dptr, void *ptr, int pos);
void		**ft_add_re_ptr(void **dptr, void *ptr, int pos);
int			ft_strcmp(char *s1, char *s2);

#endif
