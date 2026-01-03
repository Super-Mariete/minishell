/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:22 by rafael            #+#    #+#             */
/*   Updated: 2026/01/03 16:14:49 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_reset_cl(const t_read *read)
{
	size_t	i;

	i = read->cursor;
	while (i < read->line_len)
	{
		write(1, "\033[C", 3);
		i++;
	}
	while (i > 0)
	{
		write(1, "\b \b", 3);
		i--;
	}
	return ;
}

void	ft_reset_cursor(const t_read *read)
{
	size_t	i;

	i = read->line_len;
	while (i > read->cursor + 1)
	{
		write(1, "\033[D", 3);
		i--;
	}
}

void	ft_reset_read(t_read *read)
{
	size_t	i;

	i = 0;
	while (i <= read->line_len)
		read->buffer[i++] = 0;
	read->line_len = 0;
	read->cursor = 0;
}

char	*ft_get_stash(const t_read *read)
{
	char			*ret;
	unsigned long	pos_to_stash;
	unsigned long	buffer;

	buffer = (unsigned long)read->buffer;
	pos_to_stash = (BUF_MAX - read->line_len) / 2;
	ret = read->buffer + pos_to_stash;
	if (((unsigned long)ret + read->line_len) >= buffer + BUF_MAX)
	{
		write(2, NOSTASH, 44);
		return (NULL);
	}
	return (ret);
}
