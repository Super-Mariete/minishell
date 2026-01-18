/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 12:48:20 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_list(t_cli *cli)
{
	t_cli	*next;
	t_cli	*last;

	if (!cli)
		return ;
	last = cli;
	while (last->next)
		last = last->next;
	cli->status = last->status;
	next = cli->next;
	if (next)
	{
		free_list(&next);
		cli->next = nullptr;
	}
	free_first_node(cli);
}

static int	reset_signal(t_cli *cli)
{
	g_signal = 0;
	reset_list(cli);
	cli->last_status = 130;
	return (1);
}

static int	is_empty(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	process_input(const char *line, t_cli *cli)
{
	char	**tokens;

	if (is_empty(line))
		return ;
	tokens = tokenize((char *)line, cli);
	if (!tokens)
	{
		cli->last_status = 2;
		return ;
	}
	cli->status = parse_input(tokens, cli, 1, 0);
	ft_exec(cli);
}

int	read_input_line(t_shenv **env, t_cli *cli)
{
	char	*cl;

	cl = nullptr;
	while (1)
	{
		free(cl);
		cl = readline("\033[1;32mminishell\033[0m$ ");
		if (!cl)
			return (rl_clear_history(), write(1, "exit\n", 5), 2);
		if ((g_signal && reset_signal(cli)) || is_empty(cl))
			continue ;
		add_history(cl);
		process_input(cl, cli);
	}
}
