/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:12 by rafael            #+#    #+#             */
/*   Updated: 2025/12/30 22:08:54 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_print_history(t_hist *hist)
{
	unsigned char	*t;

	t = hist->buffer;
	// printf("history:\n");
	while (t < hist->last)
	{
		printf("%s\n", t);
		t += ft_strlen((char *)t) + 1;
	}
}

static unsigned char	*ft_get_next_line(t_hist *hist)
{
	unsigned char	*buffer;

	buffer = hist->current;
	while (buffer <= hist->last && *buffer != 0)
	{
		buffer++;
		if (buffer >= hist->last)
			return (NULL);
	}
	buffer++;
	hist->current = buffer;
	return (buffer);
}

static unsigned char	*ft_get_prev_line(t_hist *hist)
{
	unsigned char	*buffer;

	buffer = hist->current;
	while (buffer >= hist->buffer && *buffer != 0)
	{
		buffer--;
		if (buffer <= hist->buffer)
			return (NULL);
	}
	buffer--;
	while (buffer >= hist->buffer && *buffer != 0)
	{
		buffer--;
		if (buffer < hist->buffer)
			return (NULL);
	}
	buffer++;
	hist->current = buffer;
	return (buffer);
}

void	ft_up_history(t_read *read)
{
	unsigned const char	*next_line;
	size_t				len;
	size_t				i;

	next_line = ft_get_next_line(read->hist);
	if (!next_line)
		return ;
	i = read->cursor;
	len = ft_strlen((char *)read->hist->current);
	if (next_line < &(read->buffer[read->line_len]))
		ft_buffercpy(read->hist->current, read->buffer, len);
	read->line_len = len;
	read->cursor = len;
	while (i > 0)
	{
		write(1, "\033[D", 3);
		i--;
	}
	write(1, read->buffer, len);
}

void	ft_down_history(t_read *read)
{
	unsigned const char	*prev_line;
	size_t				len;
	size_t				i;

	prev_line = ft_get_prev_line(read->hist);
	if (!prev_line)
		return ;
	i = read->cursor;
	len = ft_strlen((char *)(read->hist->current));
	if (prev_line != read->buffer)
		ft_buffercpy(read->hist->current, read->buffer, len);
	read->line_len = len;
	read->cursor = len;
	while (i > 0)
	{
		write(1, "\033[D", 3);
		i--;
	}
	write(1, read->buffer, len);
}

void	ft_add_to_history(t_read *read)
{
	size_t	buffer_len;

	buffer_len = (read->hist->last + read->line_len) - read->hist->buffer;
	if (buffer_len >= HIST_MAX)
	{
		write(2, "minishell: history buffer out of memory\n", 40);
		return ;
	}
	ft_buffercpy(read->buffer, read->hist->last, read->line_len);
	read->hist->last += read->line_len + 1;
}
//TODO: implement refilling of line buffer