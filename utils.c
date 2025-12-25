#include "includes/minishell.h"

size_t	ft_buffercpy(char *src, char *dest, size_t size)
{
	if (!src)
		return (write(2, "minishell: no buffer found\n", 27), LONG_MAX);
	ft_strncpy(dest, src, size);
	if (*(dest + size) != 0)
		return (write(2, MEMOUT, ft_strlen(MEMOUT)), 1);
	return (0);
}