/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:56:55 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 18:48:15 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Shouldn't be necessary to check if the shell is in interactive mode
static int	ft_handle_signal(t_read *rbuffer)
{
	if (g_signal == 2)
	{
		ft_reset_read(rbuffer);
		rbuffer->hist->current = rbuffer->hist->last;
		while (rbuffer->cursor < rbuffer->line_len)
		{
			if (rbuffer->intr)
				write(STDOUT_FILENO, "\033[D", 3);
			rbuffer->cursor++;
		}
		write(2, "\n", 1);
		write(2, PROMPT, sizeof(PROMPT));
	}
	g_signal = 0;
	return (0);
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
	static char		buffer[BUF_MAX];
	static t_hist	hist;
	static char		history[HIST_MAX];
	static t_read	rbuffer;

	msh->rbuffer = &rbuffer;
	msh->rbuffer->cursor = 0;
	msh->rbuffer->buffer = buffer;
	msh->rbuffer->hist = &hist;
	msh->rbuffer->hist->buffer = history;
	msh->rbuffer->hist->current = history;
	msh->rbuffer->hist->last = history;
	msh->rbuffer->hist->stash = NULL;
	msh->rbuffer->hist->line_len = 0;
	msh->rbuffer->line_len = 0;
	if (isatty(STDIN_FILENO))
	 	msh->rbuffer->intr = 1;
	else
		msh->rbuffer->intr = 0;
	return ;
}

int	ft_readline(t_msh *msh)
{
	size_t	pos;
	char	c;

	ft_init_read(msh);
	if (msh->rbuffer->intr)
		write(2, PROMPT, sizeof(PROMPT));
	pos = 0;
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) == 0)
		{
			if (msh->rbuffer->line_len > 0 && !msh->rbuffer->intr)
				ft_process_nl(msh->rbuffer, msh);
			return (1);
		}
		if (g_signal)
		{
			ft_handle_signal(msh->rbuffer);
			if (!msh->rbuffer->intr)
				ft_parse(msh);
			continue ;
		}
		if (pos == BUF_MAX - 2)
			return (write(2, MEMOUT, sizeof(MEMOUT)), *msh->status = E2BIG, E2BIG);
		if (c == '\n')
		{
			ft_process_nl(msh->rbuffer, msh);
			pos = 0;
			continue ;
		}
		if (ft_process_key(c, msh->rbuffer, msh))
			return (1);
		pos++;
	}
	return (0);
}
