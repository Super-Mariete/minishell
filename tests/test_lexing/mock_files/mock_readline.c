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

#include "../../../minishell.h"

static void	print_tokens(char **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		printf("%s\n", tokens[i]);
		i++;
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
	char	*trimmed;
	int		mock_n;

	if (is_empty(line))
		return ;
	trimmed = trim_spaces(line);
	if (!trimmed)
		return ;
	mock_n = num_s_tokens(trimmed);
	tokens = token_sep(trimmed);
	if (!tokens)
	{
		cli->last_status = 2;
		return ;
	}
	print_tokens(tokens);
	// Parsing and Execution disabled for Lexing Unit Tests
	// cli->status = parse_input(tokens, cli, 1, 0);
	// ft_exec(cli);
	free_tokens(tokens, mock_n);
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
