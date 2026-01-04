/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:00 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 17:59:26 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_backspace(t_read *rbuffer)
{
	size_t	i;
	size_t	j;

	if (rbuffer->cursor == 0)
		return ;
	if (rbuffer->intr)
		ft_reset_cl(rbuffer);
	j = 0;
	i = 0;
	rbuffer->cursor--;
	while (i < rbuffer->line_len)
	{
		if (i == rbuffer->cursor)
		{
			i++;
			continue ;
		}
		if (rbuffer->intr)
			write(1, &(rbuffer->buffer[i]), 1);
		rbuffer->buffer[j] = rbuffer->buffer[i];
		i++;
		j++;
	}
	if (rbuffer->intr)
		ft_reset_cursor(rbuffer);
	rbuffer->buffer[j] = 0;
	rbuffer->line_len--;
	return ;
}

int	ft_process_nl(t_read *rbuffer, t_msh *msh)
{
	if (rbuffer->intr)
		write(1, "\n", 1);
	rbuffer->cursor += rbuffer->line_len;
	ft_buffercpy(rbuffer->buffer, rbuffer->hist->last, rbuffer->line_len);
	ft_add_to_history(rbuffer);
	ft_bzero(rbuffer->hist->stash, ft_strlen(rbuffer->hist->stash));
	rbuffer->hist->stash = NULL;
	ft_parse(msh);
	if (rbuffer->intr)
		write(2, PROMPT, sizeof(PROMPT));
	ft_reset_read(rbuffer);
	return (0);
}

static void	ft_add_char_mid_buffer(t_read *rbuffer, const char c)
{
	size_t	pos;
	char	temp1;
	char	temp2;

	pos = 0;
	while (pos < rbuffer->cursor)
		pos++;
	temp1 = c;
	while (pos <= rbuffer->line_len)
	{
		temp2 = rbuffer->buffer[pos];
		rbuffer->buffer[pos] = temp1;
		temp1 = temp2;
		pos++;
	}
	return ;
}

static int	ft_process_printable(const char c, t_read *rbuffer)
{
	size_t	pos;

	if (c == 127)
		return (ft_backspace(rbuffer), 1);
	if (rbuffer->cursor == rbuffer->line_len)
	{
		rbuffer->buffer[rbuffer->cursor] = c;
		if (rbuffer->intr)
			write(1, &c, 1);
		rbuffer->cursor += 1;
		rbuffer->line_len += 1;
		rbuffer->buffer[rbuffer->line_len] = 0;
		return (1);
	}
	pos = rbuffer->line_len;
	ft_add_char_mid_buffer(rbuffer, c);
	rbuffer->line_len += 1;
	rbuffer->cursor += 1;
	rbuffer->buffer[rbuffer->line_len] = 0;
	if (rbuffer->intr)
	{
		write(1, "\r", 1);
		write(2, PROMPT, sizeof(PROMPT));
		write(1, rbuffer->buffer, rbuffer->line_len);
	}
	while (pos >= rbuffer->cursor)
	{
		if (rbuffer->intr)
			write(1, "\033[D", 3);
		pos--;
	}
	return (0);
}

int	ft_process_key(const char c, t_read *rbuffer, t_msh *msh)
{
	if (c >= 32)
	{
		ft_process_printable(c, rbuffer);
		return (0);
	}
	else if (c == 27)
		ft_process_arrows(rbuffer);
	else if (c == 4 && rbuffer->intr && rbuffer->line_len == 0)
		return (write(2, "\nexit\n", 6), *(msh->status) = 2, 1);
	else if (c == 4 && !rbuffer->intr)
		return (write(2, "\nexit\n", 6), *(msh->status) = 2, 1);
	return (0);
}
