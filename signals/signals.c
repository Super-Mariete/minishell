/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:41 by rafael            #+#    #+#             */
/*   Updated: 2026/01/01 21:10:18 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_signal;

static void	ft_sig_int_parent(int signal)
{
	g_signal = 1;
	if (signal == SIGINT)
	{
		g_signal = 2;
		write(1, "^C", 2);
	}
}

// For Ctrl + z: sigaction(SIGTSTP, &sa, NULL);
void	ft_set_sig(int option)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	if (option == PARENT)
	{
		sa.sa_handler = ft_sig_int_parent;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, NULL);
	}
	if (option == CHILD)
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
	}
	if (option == IGNORE)
	{
		sa.sa_handler = SIG_IGN;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	return ;
}
