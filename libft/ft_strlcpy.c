/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:26:32 by rafael-m          #+#    #+#             */
/*   Updated: 2025/07/20 22:32:23 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <stdio.h>
 *The strlcpy() and strlcat() functions copy and concatenate strings
respectively.  They are designed to be safer, more consistent, and less error
prone replacements for strncpy(3) and strncat(3).  Unlike those functions,
strlcpy() and strlcat() take the full size of the buffer (not just the len)
and guarantee to NUL-terminate the result (as long as size is larger than 0 or,
in the case of strlcat(), as long as there is at least one byte free in dst).
Note that a byte for the NUL should be included in size.  Also note that
strlcpy() and strlcat() only operate on true “C”strings.  This means that for
strlcpy() src must be NUL-terminated and for strlcat() both src and dst must be
NUL-terminated.

The strlcpy() function copies up to size - 1 characters from the NUL-terminated
string src to dst, NUL-terminating the result.

RETURN VALUES
The strlcpy() and strlcat() functions return the total len of the string
they tried to create. For strlcpy() that means the len of src.  For
strlcat() that means the initial len of dst plus the len of src.  While
this may seem somewhat confusing, it was done to make truncation detection
simple.
*/
size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	lg;

	i = 0;
	lg = ft_strlen(src);
	if (size == 0)
		return (lg);
	while (i < (size - 1) && src[i])
	{
		dest[i] = (char)src[i];
		i++;
	}
	dest[i] = '\0';
	return (lg);
}
/*
int	main(void)
{
	char	*s = "abcd";
	char	s2[5]= {97, 112, 256 + 98,-1231234354534, 0};
	char	d[10] = "123456789";
	int	i = 0;

	ft_strlcpy(d, s, 5);
	printf("ft = %s\n", d);
	while (i < 10)
	{
		printf("ftc = %c\n", d[i]);
		i++;
	}
}*/
