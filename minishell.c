/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:16 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:17:18 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static int	add_sub_prnts(const char *line, size_t i, int *prnts)
{
	if (line[i] == '(')
	{
		(*prnts)++;
		if (line[i + 1] == '(')
		{
			write(2, "minishell: arithmetic expressions not supported\n", 49);
			return (1);
		}
	}
	if (line[i] == ')')
		(*prnts)--;
	return (0);
}

int	check_prnts(const char *line)
{
	size_t	i;
	size_t	len;
	int		prnts;

	if (!line)
		return (-1);
	prnts = 0;
	i = 0;
	while (i < ft_strlen(line))
	{
		if (ft_strchr(QUOTES, line[i]))
		{
			len = quoted_len(line + i);
			if (!len)
				return (-1);
			i += len;
			continue ;
		}
		if (add_sub_prnts(line, i, &prnts))
			return (1);
		i++;
	}
	if (prnts)
		write(2, "minishell: extra parenthesis needed\n", 37);
	return (prnts);
}

static int	event_hook(void)
{
	if (g_signal)
		rl_done = 1;
	return (0);
}

int	main(const int argc, char **argv, char **envp)
{
	t_shenv		*env;
	t_cli		*cli;
	int			status;
	int			i;

	set_sig(PARENT);
	rl_catch_signals = 0;
	rl_event_hook = event_hook;
	env = load_env(envp);
	cli = init_node(1, &env, 0);
	if (!cli)
		return (free_env(&env), 2);
	if (argc >= 2)
	{
		i = 1;
		while (i < argc)
		{
			process_input(argv[i], cli);
			i++;
		}
		status = cli->status;
	}
	else
		status = read_input_line(cli);
	return (free_list(cli), status);
}
