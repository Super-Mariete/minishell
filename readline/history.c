/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:12 by rafael            #+#    #+#             */
/*   Updated: 2026/01/03 20:01:09 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ft_print_history(t_hist *hist)
// {
// 	char	*t;

// 	t = hist->buffer;
// 	printf("history:\n");
// 	while (t < hist->last)
// 	{
// 		printf("%s\n", t);
// 		t += ft_strlen((char *)t) + 1;
// 	}
// }

static char	*ft_get_next_line(t_hist *hist)
{
	char	*buffer;

	buffer = hist->current;
	if (!(*buffer))
		return (NULL);
	while (!ft_strcmp(buffer, hist->current))
	{
		while (buffer < hist->last && *buffer != 0)
			buffer++;
		buffer++;
	}
	if (!(*buffer) && hist->stash && *(hist->stash))
		return (hist->stash);
	hist->current = buffer;
	return (buffer);
}

static char	*ft_get_prev_line(t_hist *hist)
{
	char	*buffer;

	buffer = hist->current;
	while (!ft_strcmp(buffer, hist->current))
	{
		buffer -= 2;
		while (buffer >= hist->buffer && *buffer != 0)
			buffer--;
		buffer++;
	}
	hist->current = buffer;
	return (buffer);
}

static void	ft_copy_line(const char *next_line, t_read *read)
{
	size_t	len;

	if (next_line == read->hist->stash)
	{
		len = ft_strlen(read->hist->stash);
		ft_buffercpy(read->hist->stash, read->buffer, len);
	}
	else
	{
		ft_bzero(read->buffer, read->line_len);
		len = ft_strlen(read->hist->current);
		ft_buffercpy(read->hist->current, read->buffer, len);
	}
	if (read->prompt)
		ft_reset_cl(read);
	read->line_len = len;
	read->cursor = len;
	if (read->prompt)
		write(1, read->buffer, len);
}

void	ft_down_history(t_read *read)
{
	const char	*next_line;

	next_line = ft_get_next_line(read->hist);
	if (next_line)
		ft_copy_line(next_line, read);
	else
	{
		ft_bzero(read->buffer, read->line_len);
		ft_reset_cl(read);
		read->line_len = 0;
		read->cursor = 0;
	}
	return ;
}

// TODO Better managment of filling stash buffer
void	ft_up_history(t_read *read)
{
	const char	*prev_line;
	size_t		len;

	if (read->hist->current >= read->hist->buffer && !*(read->hist->current) && *(read->buffer))
	{
		if (!read->hist->stash)
			read->hist->stash = ft_get_stash(read);
		ft_buffercpy(read->buffer, read->hist->stash, ft_strlen(read->buffer));
	}
	if (read->hist->buffer < read->hist->current)
	{
		prev_line = ft_get_prev_line(read->hist);
		if (!prev_line)
			return ;
		len = ft_strlen(read->hist->current);
		ft_buffercpy(read->hist->current, read->buffer, len);
		if (read->prompt)
			ft_reset_cl(read);
		read->line_len = len;
		read->cursor = len;
		if (read->prompt)
			write(1, read->buffer, len);
	}
	return ;
}

void	ft_add_to_history(t_read *read)
{
	size_t	buffer_len;

	if (*(read->buffer) == 0)
		return ;
	buffer_len = (read->hist->last + read->line_len) - read->hist->buffer;
	if (buffer_len >= HIST_MAX)
	{
		write(2, "minishell: history buffer out of memory\n", 40);
		return ;
	}
	ft_buffercpy(read->buffer, read->hist->last, read->line_len);
	read->hist->last += read->line_len + 1;
	read->hist->current = read->hist->last;
}
//TODO: implement refilling of line buffer