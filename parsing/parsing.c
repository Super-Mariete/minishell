/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:42 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO: properly expand filenames

static int	append_file(char *token, t_cli *cli)
{
	int	i;

	if (!token)
		return (perror_token(">>", SYN_ERR), 0);
	if (!cli)
		return (0);
	i = 0;
	if (cli->outfile)
	{
		if (!create_file(cli))
			return (0);
		free(cli->outfile);
	}
	cli->r_mode = APPEND;
	if (ft_strchr(QUOTES, token[i]))
		cli->outfile = ft_strndup(token + i + 1, ft_strlen(token) - i - 2);
	else
		cli->outfile = ft_strdup(token + i);
	if (!cli->outfile)
		return (perror("malloc : "), 0);
	return (1);
}

static int	outfile(char *token, t_cli *cli)
{
	int	i;

	if (!token)
		return (perror_token(">", SYN_ERR), 0);
	if (!cli)
		return (0);
	i = 0;
	if (cli->outfile)
	{
		if (create_file(cli))
			return (0);
	}
	free(cli->outfile);
	cli->r_mode = 0;
	if (ft_strchr(QUOTES, token[i]))
		cli->outfile = ft_strndup(token + i + 1, ft_strlen(token) - i - 2);
	else
		cli->outfile = ft_strdup(token + i);
	if (!cli->outfile)
		return (0);
	return (1);
}

static int	infile(char *token, t_cli *cli)
{
	int	i;

	if (!cli)
		return (0);
	i = 0;
	if (!token)
		return (perror_token("<", SYN_ERR), 0);
	if (cli->infile)
	{
		if (access(cli->infile, R_OK) == -1)
			return (perror(cli->infile), 0);
		free(cli->infile);
	}
	free(cli->heredoc);
	cli->heredoc = NULL;
	if (ft_strchr(QUOTES, token[i]))
		cli->infile = ft_strndup(token + i + 1, ft_strlen(token) - i - 2);
	else
		cli->infile = ft_strdup(token + i);
	if (!cli->infile)
		return (0);
	return (1);
}

static size_t	parse_input2(char **token, size_t i, t_cli *cli)
{
	if (token[i] && token[i][0] == '<')
		infile(token[++i], cli);
	else if (token[i] && token[i][0] == '>')
		outfile(token[++i], cli);
	else if (token[i] && !cli->cmd)
	{
		set_cmd(token[i], cli);
		add_args(token[i], cli, ft_doubleptr_len((void **)cli->args));
	}
	else
		add_args(token[i], cli, ft_doubleptr_len((void **)cli->args));
	return (i);
}

int	parse_input(char **tokens, t_cli *cli, size_t i)
{
	size_t	len;
	int		status;

	len = cli->n_tokens;
	cli->n_tokens = 1;
	while (i < len)
	{
		if (tokens[i] && !ft_strncmp(tokens[i], ">>", 2))
			append_file(tokens[++i], cli);
		else if (tokens[i] && !ft_strncmp(tokens[i], "<<", 2))
		{
			status = get_heredoc(tokens[++i], cli);
			if (status)
				return (free_tokens(tokens, len), status);
		}
		else if (tokens[i] && tokens[i][0] && ft_strchr(OP_STR, tokens[i][0]))
		{
			if (!parse_op(tokens[i], &cli))
				return (free_tokens(tokens, len), 2);
		}
		else
			i = parse_input2(tokens, i, cli);
		i++;
	}
	return (free_tokens(tokens, len), 0);
}
