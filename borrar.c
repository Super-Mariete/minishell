#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include "includes/minishell.h"

#define BUFFER_SIZE 1024

volatile sig_atomic_t	g_signal;

void	ft_add_char_mid_buffer(t_term *term, const char c)
{
	int		pos;
	char	temp1;
	char	temp2;

	pos = 0;
	while (pos < term->cursor)
		pos++;
	temp1 = c;
	while (pos <= term->line_len)
	{
		temp2 = term->buffer[pos];
		term->buffer[pos] = temp1;
		temp1 = temp2;
		pos++;
	}
	// printf("%s\n", term->buffer);

}

void	ft_reset_buffer(char *buffer)
{
	int	i = 0;

	while (i < BUFFER_SIZE)
		buffer[i++] = 0;
}

// Return: 1) OK,  0) EOF/signal,  -1) fatal error
int	read_key(char *c)
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

int	ft_process_arrows(t_term *term)
{
	char	esc_seq[2];

	if (read_key(&esc_seq[0]) != 1)
		return (0);
	if (esc_seq[0] != '[')
		return (0);
	if (read_key(&esc_seq[1]) != 1)
		return (0);
	else if (esc_seq[1] == 'C')
	{
		if (term->cursor < term->line_len)
		{
			term->cursor += 1;
			write(STDOUT_FILENO, "\033[C", 3);
		}
	}
	else if (esc_seq[1] == 'D' && term->cursor > 0)
	{
		if (term->cursor > 0)
		{
			term->cursor -= 1;
			write(STDOUT_FILENO, "\033[D", 3);
		}
	}
}

int	ft_process_printable(const char c, t_term *term)
{
	int	pos;

	// printf("term->cursor = %d\n", term->cursor);
	// printf("term->line_len = %d\n", term->line_len);
	if (term->cursor == term->line_len)
	{
		term->buffer[term->cursor] = c;
		write(1, &c, 1);
		term->cursor += 1;
		term->line_len += 1;
		return (1);
	}
	pos = term->line_len;
	ft_add_char_mid_buffer(term, c);
	term->line_len += 1;
	term->cursor += 1;
	write(1, "\r", 1);
	write(1, PROMPT, sizeof(PROMPT));
	write(1, term->buffer, term->line_len);
	while (pos >= term->cursor)
	{
		write(1, "\033[D", 3);
		pos--;
	}
}

int	ft_process_key(const char c, t_term *term)
{
	// write(fd, &c, 1);
	if (c >= 32 && c <= 127)
	{
		ft_process_printable(c, term);
		return (1);
	}
	if (c == '\n')
	{
		printf("%s", term->buffer);
		ft_reset_buffer(term->buffer);
		term->cursor = 0;
		term->line_len = 0;
		return (1);
	}
	if (c == 27)
		ft_process_arrows(term);
}

char	*ft_readline(t_term *term)
{
	char	c;
	size_t	bytes_read;
	int 	pos;

	pos = 0;
	write(1, PROMPT, sizeof(PROMPT));
	while (1)
	{
		if (!read_key(&c))
			return (NULL);
		if (pos == BUFFER_SIZE - 2)
			return (write(2, "END OF BUFFER\n", 14), NULL);
		if (c == '\n')
		{
			write(1, "\n", 1);
			printf("result = %s\n", term->buffer);
			write(1, PROMPT, sizeof(PROMPT));
			term->line_len = 0;
			term->cursor = 0;
			ft_reset_buffer(term->buffer);
		}
		ft_process_key(c, term);
		pos++;
	}
	return (term->buffer);
}

int	main()
{
	t_term		term;
	static char	buffer[BUFFER_SIZE];


	if (tcgetattr(STDIN_FILENO, &(term.canon_mode)) == -1)
		return (1);
	if (tcgetattr(STDIN_FILENO, &(term.raw_mode)) == -1)
		return (1);
	if (g_signal)
		g_signal = 0;
	term.raw_mode.c_lflag &= ~((tcflag_t)(ICANON | ECHO));
	term.raw_mode.c_cc[VMIN] = 1;
	term.raw_mode.c_cc[VTIME] = 0;
	term.line_len = 0;
	term.cursor = 0;
	term.buffer = buffer;
	term.history = NULL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(term.raw_mode)) == -1)
		return (-1);
	while (1)
		ft_readline(&term);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &(term.canon_mode));
}
