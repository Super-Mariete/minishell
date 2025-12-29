#include "../includes/minishell.h"

void	ft_handle_sigint(t_read *read)
{
	while (read->cursor < read->line_len)
	{
		write(STDOUT_FILENO, "\033[D", 3);
		read->cursor++;
	}
	// write(1, "\n", 1);
	// write(1, PROMPT, sizeof(PROMPT));
	// ft_reset_buffer(read);
}

void	ft_reset_buffer(t_read *read)
{
	size_t	i = 0;

	while (i < read->line_len)
		read->buffer[i++] = 0;
	read->line_len = 0;
	read->cursor = 0;
}

int	read_key(unsigned char *c)
{
	ssize_t	ret;

	ret = read(STDIN_FILENO, c, 1);
	if (ret == -1)
	{
		if (errno == EINTR)
			return (0);
		return (perror("read: "), -1);
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
	static unsigned char	history[HIST_MAX];
	static t_read			read;

	msh->read = &read;
	msh->read->cursor = 0;
	msh->read->buffer = buffer;
	msh->read->history = history;
	msh->read->line_len = 0;
	return ;
}

int	ft_readline(t_msh *msh)
{
	unsigned char	c;
	size_t 			pos;

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
			{
				ft_handle_sigint(msh->read);
				c = '\n';
			}
			g_signal = 0;
		}
		if (pos == READ_MAX - 2)
			return (write(2, MEMOUT, sizeof(MEMOUT)), E2BIG);
		if (c == '\n')
		{
			write(1, "\n", 1);
			printf("result = %s\n", msh->read->buffer);
			write(1, PROMPT, sizeof(PROMPT));
			ft_reset_buffer(msh->read);
			continue ;
		}
		if (ft_process_key(c, msh->read) == 2)
			return (2);
		pos++;
	}
	return (0);
}