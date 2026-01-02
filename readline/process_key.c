/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:00 by rafael            #+#    #+#             */
/*   Updated: 2026/01/02 00:38:06 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_backspace(t_read *read)
{
	size_t	i;
	size_t	j;

	if (read->cursor == 0)
		return ;
	ft_reset_cl(read);
	j = 0;
	i = 0;
	read->cursor--;
	while (i < read->line_len)
	{
		if (i == read->cursor)
		{
			i++;
			continue ;
		}
		write(1, &(read->buffer[i]), 1);
		read->buffer[j] = read->buffer[i];
		i++;
		j++;
	}
	ft_reset_cursor(read);
	read->buffer[j] = 0;
	read->line_len--;
	return ;
}

int	ft_process_nl(t_read *read, const unsigned char c)
{
	if (c == '\n')
	{
		write(1, "\n", 1);
		printf("result = %s\n", read->buffer);
		read->cursor += read->line_len;
		ft_buffercpy(read->buffer, read->hist->last, read->line_len);
		ft_add_to_history(read);
		ft_print_history(read->hist);
		printf("stash = %s\n", read->hist->stash);
		ft_bzero(read->hist->stash, ft_strlen((char *)read->hist->stash));
		read->hist->stash = NULL;
		write(1, PROMPT, sizeof(PROMPT));
		ft_reset_buffer(read);
		return (1);
	}
	return (0);
}

static void	ft_add_char_mid_buffer(t_read *read, const unsigned char c)
{
	size_t			pos;
	unsigned char	temp1;
	unsigned char	temp2;

	pos = 0;
	while (pos < read->cursor)
		pos++;
	temp1 = c;
	while (pos <= read->line_len)
	{
		temp2 = read->buffer[pos];
		read->buffer[pos] = temp1;
		temp1 = temp2;
		pos++;
	}
	return ;
}

static int	ft_process_printable(const unsigned char c, t_read *read)
{
	size_t	pos;

	if (c == 127)
		return (ft_backspace(read), 1);
	if (read->cursor == read->line_len)
	{
		read->buffer[read->cursor] = c;
		write(1, &c, 1);
		read->cursor += 1;
		read->line_len += 1;
		return (1);
	}
	pos = read->line_len;
	ft_add_char_mid_buffer(read, c);
	read->line_len += 1;
	read->cursor += 1;
	write(1, "\r", 1);
	write(1, PROMPT, sizeof(PROMPT));
	write(1, read->buffer, read->line_len);
	while (pos >= read->cursor)
	{
		write(1, "\033[D", 3);
		pos--;
	}
	return (0);
}

int	ft_process_key(const unsigned char c, t_read *read)
{
	if (c >= 32 && c <= 127)
	{
		ft_process_printable(c, read);
		return (0);
	}
	if (c == '\n')
	{
		printf("%s", read->buffer);
		ft_reset_buffer(read);
		return (0);
	}
	else if (c == 27)
		ft_process_arrows(read);
	else if (c == 4 && read->line_len == 0)
		return (write(2, "\nexit\n", 6), 2);
	else if (c == 4)
		return (0);
	return (0);
}
