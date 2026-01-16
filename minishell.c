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

int	check_prnts(char *line)
{
	int		i;
	int		prnts;

	if (!line)
		return (-1);
	i = 0;
	prnts = 0;
	while (i < (int)ft_strlen(line))
	{
		if (ft_strchr(QUOTES, line[i])
			&& (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			if (quoted_len(line + i, line[i]) < 0)
				return (-1);
			i += (quoted_len(line + i, line[i]) - 1);
		}
		if (line[i] == '(')
			prnts++;
		if (line[i] == ')')
			prnts--;
		i++;
	}
	if (prnts)
		write(2, "minishell : extra parenthesis\n", 30);
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
		status = cli->last_status;
	}
	else
		status = read_input_line(&env, cli);
	free_list(&cli);
	free_env(&env);
	return (status);
}
