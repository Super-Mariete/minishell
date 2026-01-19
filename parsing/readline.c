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

static void     print_op(int op)
{
	if (op == PIPE)
		printf("PIPE (|)\n");
	else if (op == AND)
		printf("AND (&&)\n");
	else if (op == OR)
		printf("OR (||)\n");
	else
		printf("NONE\n");
}

static void     print_args(char **args)
{
	int             i;

	printf("Args: [");
	if (args)
	{
		i = 0;
		while (args[i])
		{
			printf("'%s'", args[i]);
			if (args[i + 1])
				printf(", ");
			i++;
		}
	}
	printf("]\n");
}
void    print_parser(t_cli *cli)
{
	t_cli   *node;
	int             index;

	if (!cli)
		return ;
	node = cli;
	index = 0;
	while (node)
	{
		printf("--- Node %d ---\n", index++);
		printf("Group: %zu\n", node->group);
		printf("Op: ");
		print_op(node->op);
		printf("Cmd: %s\n", node->cmd ? node->cmd : "(null)");
		print_args(node->args);
		printf("Infile: %s\n", node->infile ? node->infile : "(null)");
		printf("Outfile: %s\n", node->outfile ? node->outfile : "(null)");
		printf("Heredoc: %s\n", node->heredoc ? node->heredoc : "(null)");
		printf("R_mode: %d\n", node->r_mode);
		printf("Is_builtin: %d\n", node->is_builtin);
		node = node->next;
	}
	printf("--- End of List ---\n");
}
void	reset_list(t_cli *cli)
{
	t_cli	*next;
	t_cli	*t;
	t_cli	*last;
	t_cli	*first;

	if (!cli)
		return ;
	last = cli;
	first = cli;
	while (last->next)
		last = last->next;
	while (first->prev)
		first = first->prev;
	cli->status = last->status;
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
	cli->last_status = parse_input(tokens, cli, 1, 0);
	if (cli->last_status)
	{
		reset_list(cli);
		return ;
	}
	while (cli->prev)
		cli = cli->prev;
	print_parser(cli);
	ft_exec(cli);
}

int	read_input_line(t_cli *cli)
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
		// reset_list(cli);
	}
}
