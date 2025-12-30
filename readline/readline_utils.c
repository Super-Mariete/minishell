#include "../includes/minishell.h"

void	ft_reset_cl(t_read *read)
{
	size_t	i;

	i = read->cursor;
	while (i < read->line_len)
	{
		write(1, "\033[C", 3);
		i++;
	}
	while (i > 0)
	{
		write(1, "\b", 1);
		write(1, " ", 1);
		write(1, "\b", 1);
		i--;
	}
	return ;
}

void	ft_reset_cursor(t_read *read)
{
	size_t	i;
	
	i = read->line_len;
	while (i > read->cursor + 1)
	{
		write(1, "\033[D", 3);
		i--;
	}
}