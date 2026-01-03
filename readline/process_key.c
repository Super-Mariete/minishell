/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:00 by rafael            #+#    #+#             */
/*   Updated: 2026/01/03 20:23:04 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_backspace(t_read *read)
{
	size_t	i;
	size_t	j;

	if (read->cursor == 0)
		return ;
	if (read->prompt)
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
		if (read->prompt)
			write(1, &(read->buffer[i]), 1);
		read->buffer[j] = read->buffer[i];
		i++;
		j++;
	}
	if (read->prompt)
		ft_reset_cursor(read);
	read->buffer[j] = 0;
	read->line_len--;
	return ;
}

int	ft_process_nl(t_read *read, const char c, t_msh *msh)
{
	// if (c == '\n' && read->cursor > 0 && ft_strchr(QUOTES, *(&read->buffer[read->cursor - 1])))
	// {
	// 	read->buffer[read->cursor] = c;
	// 	read->cursor++;
	// 	read->line_len++;
	// 	return (0);
	// }
	if (c == '\n')
	{
		if (read->prompt)
			write(1, "\n", 1);
		read->cursor += read->line_len;
		ft_parse(msh);
		ft_buffercpy(read->buffer, read->hist->last, read->line_len);
		ft_add_to_history(read);
		ft_bzero(read->hist->stash, ft_strlen(read->hist->stash));
		read->hist->stash = NULL;
		if (read->prompt)
			write(2, PROMPT, sizeof(PROMPT));
		if (read->prompt)
			ft_reset_read(read);
		return (1);
	}
	return (0);
}

static void	ft_add_char_mid_buffer(t_read *read, const char c)
{
	size_t	pos;
	char	temp1;
	char	temp2;

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

static int	ft_process_printable(const char c, t_read *read)
{
	size_t	pos;

	if (c == 127)
		return (ft_backspace(read), 1);
	if (read->cursor == read->line_len)
	{
		read->buffer[read->cursor] = c;
		if (read->prompt)
			write(1, &c, 1);
		read->cursor += 1;
		read->line_len += 1;
		return (1);
	}
	pos = read->line_len;
	ft_add_char_mid_buffer(read, c);
	read->line_len += 1;
	read->cursor += 1;
	if (read->prompt)
	{
		write(1, "\r", 1);
		write(2, PROMPT, sizeof(PROMPT));
		write(1, read->buffer, read->line_len);
	}
	while (pos >= read->cursor)
	{
		if (read->prompt)
			write(1, "\033[D", 3);
		pos--;
	}
	return (0);
}

int	ft_process_key(const char c, t_read *read)
{
	if (c >= 32)
	{
		ft_process_printable(c, read);
		return (0);
	}
	else if (c == 27)
		ft_process_arrows(read);
	else if (c == 4 && read->line_len == 0)
		return (write(2, "\nexit\n", 6), 2);
	else if (c == 4 && read->prompt)
		return (0);
	else if (c == 4 && !read->prompt)
		return (printf("HOLA\n"), 1);
	return (0);
}
