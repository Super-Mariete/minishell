#include "includes/minishell.h"

size_t	ft_buffercpy(const char *src, char *dest, size_t size)
{
	// TODO: implement buffer rewrite when reached the limit and check again
	//to fill gaps.
	ft_strncpy(dest, src, size);
	if (*(dest + size) != 0)
		return (1);
	return (0);
}