/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_list(t_cli *cli)
{
	t_cli		*next;
	t_cli		*t;
	const t_cli	*last;
	t_cli		*first;

	if (!cli)
		return ;
	last = cli;
	first = cli;
	while (last->next)
		last = last->next;
	while (first->prev)
		first = first->prev;
	first->status = last->status;
	next = cli->next;
	while (next)
	{
		t = next->next;
		free_node(next);
		next = t;
	}
	free_first_node(first);
}

static int	reset_signal(t_cli *cli)
{
	g_signal = 0;
	reset_list(cli);
	cli->status = 130;
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
		cli->status = 2;
		return ;
	}
	cli->status = 0;
	cli->status = parse_input(tokens, cli, 0);
	if (cli->status)
	{
		reset_list(cli);
		return ;
	}
	while (cli->prev)
		cli = cli->prev;
	if (cli->status == 0)
		execute(cli);
	reset_list(cli);
}

int	read_input_line(t_cli *cli)
{
	char	*cl;

	cl = NULL;
	while (1)
	{
		free(cl);
		cl = readline("\001\033[1;32m\002minishell\001\033[0m\002$ ");
		if (!cl)
			return (rl_clear_history(), write(1, "exit\n", 5), 2);
		if ((g_signal && reset_signal(cli)) || is_empty(cl))
			continue ;
		add_history(cl);
		process_input(cl, cli);
	}
}
