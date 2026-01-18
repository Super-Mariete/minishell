/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:33:56 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_heredoc(const int option, t_cli *cli)
{
	char	*t;
	char	**herearray;

	if (!option)
	{
		herearray = ft_split(cli->heredoc, '\n');
		if (!herearray)
			return (NULL);
		herearray = expand_array(herearray, cli);
		if (!herearray)
			return (NULL);
		t = convert_to_string(herearray);
		free(cli->heredoc);
		cli->heredoc = t;
	}
	return (cli->heredoc);
}

void	here_error(const char *delim)
{
	char	*t;
	char	*error_msg;

	t = ft_strjoin(HERE_ERR, delim);
	error_msg = ft_strjoin(t, "')\n");
	if (!error_msg)
	{
		perror("malloc : ");
		return ;
	}
	write(2, error_msg, ft_strlen(error_msg));
	free(error_msg);
	free(t);
}

static void	free_prev(t_cli *cli)
{
	free(cli->heredoc);
	free(cli->infile);
	cli->infile = nullptr;
	cli->heredoc = nullptr;
}

static int	read_heredoc(t_cli *cli, const int *option, char *delim)
{
	char	*line;
	char	*t;

	line = nullptr;
	while (1)
	{
		free(line);
		line = readline("> ");
		if (g_signal)
			return (free(line), free(delim), cli->status = 130, 130);
		if (!line || (ft_strlen(line) == ft_strlen(delim)
				&& !ft_strcmp(line, delim)))
			break ;
		t = ft_strjoin(cli->heredoc, line);
		free(cli->heredoc);
		cli->heredoc = ft_strjoin(t, "\n");
		free(t);
	}
	if (!line)
		here_error(delim);
	cli->heredoc = expand_heredoc(*option, cli);
	if (!cli->heredoc)
		return (cli->status = 2, 2);
	return (free(line), free(delim), 0);
}

int	get_heredoc(const char *token, t_cli *cli)
{
	char	*delim;
	int		option;
	int		status;

	if (!cli)
		return (2);
	free_prev(cli);
	if (!token)
		return (perror_token("<<", SYN_ERR), 2);
	option = 0;
	delim = trim_delim(token, &option);
	if (!delim)
		return (cli->status = 2, 2);
	status = read_heredoc(cli, &option, delim);
	if (status == 130)
	{
		set_sig(PARENT);
		g_signal = 0;
		cli->last_status = 130;
	}
	cli->heredoc_fd = create_heredoc(cli);
	return (status);
}
