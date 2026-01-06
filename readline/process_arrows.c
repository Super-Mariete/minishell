/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arrows.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:57:03 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 10:23:05 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_fwd_cursor(t_read *rbuffer)
{
	if (rbuffer->cursor < rbuffer->line_len)
	{
		rbuffer->cursor += 1;
		if (rbuffer->intr)
			write(STDOUT_FILENO, "\033[C", 3);
	}
}

static void	ft_bwd_cursor(t_read *rbuffer)
{
	if (rbuffer->cursor > 0)
	{
		rbuffer->cursor -= 1;
		if (rbuffer->intr)
			write(STDOUT_FILENO, "\033[D", 3);
	}
}

int	ft_process_arrows(t_read *rbuffer)
{
	char	esc_seq[2];

	if (read(STDIN_FILENO, &esc_seq[0], 1) != 1)
		return (0);	
	if (esc_seq[0] != '[')
		return (0);
	if (read(STDIN_FILENO, &esc_seq[1], 1) != 1)
		return (0);
	if (esc_seq[1] == 'C')
		ft_fwd_cursor(rbuffer);
	else if (esc_seq[1] == 'D')
		ft_bwd_cursor(rbuffer);
	else if (esc_seq[1] == 'A')
		ft_up_history(rbuffer);
	else if (esc_seq[1] == 'B')
		ft_down_history(rbuffer);
	return (0);
}
