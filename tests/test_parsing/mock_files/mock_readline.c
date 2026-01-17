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

void	print_parser(t_cli *cli)
{
	t_cli	*node;
	size_t	i;

	if (!cli)
		return ;
	node = cli;
	while (node)
	{
		printf("%s\n", node->cmd);
		i = 0;
		while (node->args &&  node->args[i])
		{
			printf("%s\n", node->args[i]);
			i++;
		}
		printf("%s\n", node->heredoc);
		printf("%s\n", node->outfile);
		printf("%s\n", node->infile);
		printf("%d\n", node->is_builtin);
		printf("%d\n", node->r_mode);
		node = node->next;
	}
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

int	read_input_line(t_shenv **env, t_cli *cli)
{
	char	*cl;
	size_t	len;
	ssize_t	n;
    (void)env;

	cl = nullptr;
	len = 0;
	while (1)
	{
		free(cl);
		cl = nullptr;
		n = getline(&cl, &len, stdin);
		if (n == -1)
		{
			free(cl);
			cl = nullptr;
		}
		else if (n > 0 && cl[n - 1] == '\n')
			cl[n - 1] = '\0';

		if (!cl)
			return (rl_clear_history(), write(1, "exit\n", 5), 2);
		if ((g_signal && reset_signal(cli)) || is_empty(cl))
			continue ;
		add_history(cl);
		process_input(cl, cli);
	}
}
