/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:12 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 18:55:18 by rafael           ###   ########.fr       */
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

static void	ft_copy_line(const char *next_line, t_read *rbuffer)
{
	size_t	len;

	if (next_line == rbuffer->hist->stash)
	{
		len = ft_strlen(rbuffer->hist->stash);
		ft_buffercpy(rbuffer->hist->stash, rbuffer->buffer, len);
	}
	else
	{
		ft_bzero(rbuffer->buffer, rbuffer->line_len);
		len = ft_strlen(rbuffer->hist->current);
		ft_buffercpy(rbuffer->hist->current, rbuffer->buffer, len);
	}
	if (rbuffer->intr)
		ft_reset_cl(rbuffer);
	rbuffer->line_len = len;
	rbuffer->cursor = len;
	if (rbuffer->intr)
		write(1, rbuffer->buffer, len);
}

void	ft_down_history(t_read *rbuffer)
{
	const char	*next_line;

	next_line = ft_get_next_line(rbuffer->hist);
	if (next_line)
		ft_copy_line(next_line, rbuffer);
	else
	{
		ft_bzero(rbuffer->buffer, rbuffer->line_len);
		ft_reset_cl(rbuffer);
		rbuffer->line_len = 0;
		rbuffer->cursor = 0;
	}
	return ;
}

// TODO Better managment of filling stash buffer
void	ft_up_history(t_read *rbuffer)
{
	const char	*prev_line;
	size_t		len;

	if (rbuffer->hist->current >= rbuffer->hist->buffer && !*(rbuffer->hist->current) && *(rbuffer->buffer))
	{
		if (!rbuffer->hist->stash)
			rbuffer->hist->stash = ft_get_stash(rbuffer);
		ft_buffercpy(rbuffer->buffer, rbuffer->hist->stash, ft_strlen(rbuffer->buffer));
	}
	if (rbuffer->hist->buffer < rbuffer->hist->current)
	{
		prev_line = ft_get_prev_line(rbuffer->hist);
		if (!prev_line)
			return ;
		len = ft_strlen(rbuffer->hist->current);
		ft_buffercpy(rbuffer->hist->current, rbuffer->buffer, len);
		if (rbuffer->intr)
			ft_reset_cl(rbuffer);
		rbuffer->line_len = len;
		rbuffer->cursor = len;
		if (rbuffer->intr)
			write(1, rbuffer->buffer, len);
	}
	return ;
}

void	ft_add_to_history(t_read *rbuffer)
{
	size_t	buffer_len;
	size_t	i;

	if (*(rbuffer->buffer) == 0)
		return ;
	buffer_len = (rbuffer->hist->last + rbuffer->line_len) - rbuffer->hist->buffer;
	if (buffer_len >= HIST_MAX)
	{
		i = 0;
		while (i < HIST_MAX)
		{
			rbuffer->hist->buffer[i] = 0;
			i++;
			rbuffer->hist->current = rbuffer->hist->buffer;
			rbuffer->hist->last = rbuffer->hist->buffer;
			rbuffer->hist->line_len = 0;
 		}
		return ;
	}
	ft_buffercpy(rbuffer->buffer, rbuffer->hist->last, rbuffer->line_len);
	rbuffer->hist->last += rbuffer->line_len + 1;
	rbuffer->hist->current = rbuffer->hist->last;
}
//TODO: implement refilling of line buffer