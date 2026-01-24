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
	print_tokens(tokens);
	// Parsing and Execution disabled for Lexing Unit Tests
	// cli->status = parse_input(tokens, cli, 1, 0);
	// ft_exec(cli);

	// Assuming free_tokens handles the array from tokenize
	// We need to know the length if free_tokens requires it, 
	// or if it's NULL-terminated. 
	// Minishell.h: void free_tokens(char **tokens, size_t n);
	// We need 'n'. 
	// tokenize implementation isn't visible here, but usually it returns a NULL terminated array 
	// AND sets cli->n_tokens? Or we calculate it?
	// The original mock used `num_s_tokens`.
	// Let's count them for freeing if free_tokens needs N.
	size_t n = 0;
	while (tokens[n]) n++;
	free_tokens(tokens, n);
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