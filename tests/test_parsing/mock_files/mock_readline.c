/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemini <gemini@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 00:00:00 by gemini            #+#    #+#             */
/*   Updated: 2026/01/16 00:00:00 by gemini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	print_op(int op)
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

static void	print_args(char **args)
{
	int		i;

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

void	print_parser(t_cli *cli)
{
	t_cli	*node;
	int		index;

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
	print_parser(cli);
	reset_list(cli);
}

int	read_input_line(t_cli *cli)
{
	char	*cl;
	size_t	len;
	ssize_t	n;

	cl = nullptr;
	len = 0;
	while (1)
	{
		free(cl);
		cl = nullptr;
		len = 0;
		n = getline(&cl, &len, stdin);
		if (n == -1)
		{
			free(cl);
			cl = nullptr;
			break ;
		}
		if (n > 0 && cl[n - 1] == '\n')
			cl[n - 1] = '\0';
		if ((g_signal && reset_signal(cli)) || is_empty(cl))
			continue ;
		add_history(cl);
		process_input(cl, cli);
	}
	write(1, "exit\n", 5);
	return (2);
}
