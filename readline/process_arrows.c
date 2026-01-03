/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arrows.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:03 by rafael            #+#    #+#             */
/*   Updated: 2026/01/03 19:53:43 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_fwd_cursor(t_read *read)
{
	if (read->cursor < read->line_len)
	{
		read->cursor += 1;
		if (read->prompt)
			write(STDOUT_FILENO, "\033[C", 3);
	}
}

static void	ft_bwd_cursor(t_read *read)
{
	if (read->cursor > 0)
	{
		read->cursor -= 1;
		if (read->prompt)
			write(STDOUT_FILENO, "\033[D", 3);
	}
}

int	ft_process_arrows(t_read *read)
{
	char	esc_seq[2];

	if (read_key(&esc_seq[0]) != 1)
		return (0);
	if (esc_seq[0] != '[')
		return (0);
	if (read_key(&esc_seq[1]) != 1)
		return (0);
	if (esc_seq[1] == 'C')
		ft_fwd_cursor(read);
	else if (esc_seq[1] == 'D')
		ft_bwd_cursor(read);
	else if (esc_seq[1] == 'A')
		ft_up_history(read);
	else if (esc_seq[1] == 'B')
		ft_down_history(read);
	return (0);
}
