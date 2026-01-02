/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:56:55 by rafael            #+#    #+#             */
/*   Updated: 2026/01/02 00:34:52 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_handle_sigint(t_read *read, unsigned char *c)
{
	*c = '\n';
	ft_reset_buffer(read);
	while (read->cursor < read->line_len)
	{
		write(STDOUT_FILENO, "\033[D", 3);
		read->cursor++;
	}
}

int	read_key(unsigned char *c)
{
	ssize_t	ret;

	ret = read(STDIN_FILENO, c, 1);
	if (ret == -1)
	{
		if (errno == EINTR)
			return (0);
		return (perror("minishell: read: "), -1);
	}
	if (ret == 0)
		return (0);
	return (1);
}

int	ft_init_term(t_term *term)
{
	if (tcgetattr(STDIN_FILENO, &(term->canon_mode)) == -1)
		return (1);
	if (tcgetattr(STDIN_FILENO, &(term->raw_mode)) == -1)
		return (1);
	term->raw_mode.c_lflag &= ~((tcflag_t)(ICANON | ECHO));
	term->raw_mode.c_cc[VMIN] = 1;
	term->raw_mode.c_cc[VTIME] = 0;
	return (0);
}

static void	ft_init_read(t_msh *msh)
{
	static unsigned char	buffer[READ_MAX];
	static t_hist			hist;
	static unsigned char	history[HIST_MAX];
	static t_read			read;

	msh->read = &read;
	msh->read->cursor = 0;
	msh->read->buffer = buffer;
	msh->read->hist = &hist;
	msh->read->hist->buffer = history;
	msh->read->hist->current = history;
	msh->read->hist->last = history;
	msh->read->hist->stash = NULL;
	msh->read->hist->line_len = 0;
	msh->read->line_len = 0;
	return ;
}

int	ft_readline(t_msh *msh)
{
	unsigned char	c;
	size_t			pos;

	pos = 0;
	ft_init_read(msh);
	write(1, PROMPT, sizeof(PROMPT));
	while (1)
	{
		if (!read_key(&c) && g_signal != 2)
			return (2);
		if (g_signal)
		{
			if (g_signal == 2)
				ft_handle_sigint(msh->read, &c);
			g_signal = 0;
		}
		if (pos == READ_MAX - 2)
			return (write(2, MEMOUT, sizeof(MEMOUT)), E2BIG);
		if (ft_process_nl(msh->read, c))
			continue ;
		if (ft_process_key(c, msh->read) == 2)
			return (2);
		pos++;
	}
	return (0);
}
