/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:22 by rafael            #+#    #+#             */
/*   Updated: 2025/12/30 21:20:13 by rafael           ###   ########.fr       */
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
		write(1, "\b", 1);
		write(1, " ", 1);
		write(1, "\b", 1);
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

void	ft_reset_buffer(t_read *read)
{
	size_t	i;

	i = 0;
	while (i < read->line_len)
		read->buffer[i++] = 0;
	read->line_len = 0;
	read->cursor = 0;
}
