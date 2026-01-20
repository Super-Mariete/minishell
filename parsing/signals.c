/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 11:16:13 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sig_int_parent(const int signal)
{
	char	nl;

	nl = '\n';
	g_signal = 1;
	if (signal == SIGINT)
	{
		write(1, "^C", 2);
		ioctl(STDIN_FILENO, TIOCSTI, &nl);
	}
}

void	set_sig(int option)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	if (option == PARENT)
	{
		sa.sa_handler = sig_int_parent;
		sigaction(SIGINT, &sa, nullptr);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, nullptr);
	}
	if (option == CHILD)
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, nullptr);
		sigaction(SIGQUIT, &sa, nullptr);
	}
	if (option == IGNORE)
	{
		sa.sa_handler = SIG_IGN;
		sigaction(SIGINT, &sa, nullptr);
		sigaction(SIGQUIT, &sa, nullptr);
	}
}
