/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:22 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 01:59:04 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_reset_cl(const t_read *rbuffer)
{
	size_t	i;

	i = rbuffer->cursor;
	while (i < rbuffer->line_len)
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

void	ft_reset_cursor(const t_read *rbuffer)
{
	size_t	i;

	i = rbuffer->line_len;
	while (i > rbuffer->cursor + 1)
	{
		write(1, "\033[D", 3);
		i--;
	}
}

void	ft_reset_read(t_read *rbuffer)
{
	size_t	i;

	i = 0;
	while (i <= rbuffer->line_len + 1)
	{
		rbuffer->buffer[i] = 0;
		i++;
	}
	rbuffer->line_len = 0;
	rbuffer->cursor = 0;
}

char	*ft_get_stash(const t_read *rbuffer)
{
	char			*ret;
	unsigned long	pos_to_stash;
	unsigned long	buffer;

	buffer = (unsigned long)rbuffer->buffer;
	pos_to_stash = (BUF_MAX - rbuffer->line_len) / 2;
	ret = rbuffer->buffer + pos_to_stash;
	if (((unsigned long)ret + rbuffer->line_len) >= buffer + BUF_MAX)
	{
		write(2, NOSTASH, 44);
		return (NULL);
	}
	return (ret);
}
