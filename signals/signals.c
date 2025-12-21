#include "../includes/minishell.h"

sig_atomic_t g_signal;

static void	ft_sig_int_parent(int signal)
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
		sigaction(SIGQUIT, &sa, NULL);
	}
	if (option == IGNORE)
	{
		sa.sa_handler = SIG_IGN;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	return ;
}