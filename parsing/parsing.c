/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:42 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/04 18:46:40 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	append(char *token, t_cli *cli)
{
	int	i;
	int fd;

	if (!token)
		return (perror_token(">>", SYN_ERR), 0);
	if (!cli)
		return (0);
	i = 0;
	if (cli->outfile)
	{
		fd = open(cli->outfile, O_WRONLY | O_CREAT | (cli->r_mode == APPEND ? O_APPEND : O_TRUNC), 0644);
		if (fd < 0)
			return (perror(cli->outfile), 0);
		close(fd);
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

int	outfile(char *token, t_cli *cli)
{
	int	i;
	int fd;

	if (!token)
		return (perror_token(">", SYN_ERR), 0);
	if (!cli)
		return (0);
	i = 0;
	if (cli->outfile)
	{
		fd = open(cli->outfile, O_WRONLY | O_CREAT | (cli->r_mode == APPEND ? O_APPEND : O_TRUNC), 0644);
		if (fd < 0)
			return (perror(cli->outfile), 0);
		close(fd);
	}
	free(cli->outfile);
	cli->r_mode = 0;
	if (ft_strchr(QUOTES, token[i]) && (i == 0 || (i > 0 && token[i - 1] != '\\')))
		cli->outfile = ft_strndup(token + i + 1, ft_strlen(token) - i - 2);
	else
		cli->outfile = ft_strdup(token + i);
	if (!cli->outfile)
		return (0);
	return (1);
}

int	infile(char *token, t_cli *cli)
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
	cli->heredoc = nullptr;
	if (ft_strchr(QUOTES, token[i]))
		cli->infile = ft_strndup(token + i + 1, ft_strlen(token) - i - 2);
	else
		cli->infile = ft_strdup(token + i);
	if (!cli->infile)
		return (0);
	return (1);
}

int	parse_token(char **token, int i, t_cli *cli, int *group)
{
	if (token[i] && token[i][0] == '<')
		infile(token[++i], cli);
	else if (token[i] && token[i][0] == '>')
		outfile(token[++i], cli);
	else if (token[i] && token[i][0] == '(')
		(*group)++;
	else if (token[i] && token[i][0] == ')')
	{
		(*group)--;
		cli->op = 0;
	}
	else if (token[i] && !cli->cmd)
	{
		set_cmd(token[i], cli);
		add_args(token[i], cli, ft_doubleptr_len((void **)cli->args));
		cli->group = *group;
	}
	else
		add_args(token[i], cli, ft_doubleptr_len((void **)cli->args));
	return (i);
}

int	parse_input(char **token, t_cli *cli)
{
	int		i;
	int		len;
	int		status;
	int		group;

	if (!token || !cli)
		return (2);
	i = 0;
	group = 1;
	len = cli->n_tokens;
	cli->n_tokens = 1;
	while (i < len)
	{
		if (token[i] && !ft_strncmp(token[i], ">>", 2))
			append(token[++i], cli);
		else if (token[i] && !ft_strncmp(token[i], "<<", 2))
		{
			if (heredoc(token[++i], cli) == 130)
				return (130);
		}
		else if (token[i] && ft_strchr(OP_STR2, token[i][0]))
		{
			cli->next = parse_op(token[i], cli);
			if (!cli->next)
				return (2);
			cli = cli->next;
		}
		else
			i = parse_token(token, i, cli, &group);
		i++;
	}
	return (free_tokens(token, len), 0);
}