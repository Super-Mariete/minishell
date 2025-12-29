#include "../includes/minishell.h"

void	ft_add_char_mid_buffer(t_read *read, const unsigned char c)
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
	// printf("%s\n", term->buffer);

}

int	ft_process_arrows(t_read *read)
{
	unsigned char	esc_seq[2];

	if (read_key(&esc_seq[0]) != 1)
		return (0);
	if (esc_seq[0] != '[')
		return (0);
	if (read_key(&esc_seq[1]) != 1)
		return (0);
	else if (esc_seq[1] == 'C')
	{
		if (read->cursor < read->line_len)
		{
			read->cursor += 1;
			write(STDOUT_FILENO, "\033[C", 3);
		}
	}
	else if (esc_seq[1] == 'D' && read->cursor > 0)
	{
		if (read->cursor > 0)
		{
			read->cursor -= 1;
			write(STDOUT_FILENO, "\033[D", 3);
		}
	}
	return (0);
}

int	ft_process_printable(const unsigned char c, t_read *read)
{
	size_t	pos;

	// printf("term->cursor = %d\n", term->cursor);
	// printf("term->line_len = %d\n", term->line_len);
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
	if (c == 27)
		ft_process_arrows(read);
	if (c == 4 && read->line_len == 0)
		return (write(2, "exit\n", 5), 2);
	else if (c == 4)
		return (0);
	return (0);
}