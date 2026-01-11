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

int	heredoc_len(const char *line)
{
	char	redir;
	int		i;
	int		len;
	
	i = 0;
	while (i < ft_strlen(line) && i < 2 && ft_strchr(REDIR_S, line[i]))
		i++;
	while (line[i] && ft_isspace(line[i]))
		i++;
	while (i < ft_strlen(line) && line[i])
	{
		if (i < ft_strlen(line) && ft_strchr(QUOTES, line[i])  && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			len = quoted_len(line + i, line[i]);
			if (len <= 0)
				return (-1);
			i += (len + 1);
			continue ;
		}	
		if (ft_strchr(SEP_STR, line [i]))
			return (i);
		i++;
	}
	return (i);
}

char	*expand_heredoc(int option, t_cli *cli)
{
	char	*t;

	t = nullptr;
	if (option)
	{
		t = expand_line(cli->heredoc, cli);
		if (!t)
			return (nullptr);
		cli->heredoc = t;
	}
	return (cli->heredoc);
}

void	here_error(char *delim)
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

// char	*heredoc_op(char *line, char op)
// {
// 	char	*new_line;
// 	char	*t;
// 	int		i;

// 	if (!line)
// 		return (NULL);
// 	new_line = NULL;
// 	while (1)
// 	{
// 		i = 0;
// 		free(new_line);
// 		new_line = readline("> ");
// 		if (g_sig_rec)
// 			return (free(new_line), line);
// 		if (!new_line)
// 			return (free(line), line = NULL, write(2, HERE_PIPE_ERR, 53), NULL);
// 		while (new_line && ft_isspace(new_line[i]))
// 			i++;
// 		if (!new_line[i] || new_line[i] == '\n')
// 			continue ;
// 		t = ft_strjoin(line, new_line);
// 		free(line);
// 		line = t;
// 		if (ft_strchr(OP_STR2, line[ft_strlen(line) - 1]))
// 			continue ;
// 		break ;
// 	}
// 	return (free(new_line), line);
// }

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
		if (!line || !ft_strncmp(line, delim, ft_strlen(line)))
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

int	heredoc(char *token, t_cli *cli)
{
	char	*delim;
	int		option;
	int		status;

	if (!cli)
		return (printf("!cli\n"), 2);
	free_prev(cli);
	if (!token)
		return (perror_token("<<", SYN_ERR), 2);
	delim = trim_delim(token, &option);
	if (!delim)
		return (cli->status = 2, 2);
	option = 0;
	status = read_heredoc(cli, &option, delim);
	if (status == 130)
	{
		set_sig(PARENT);
		g_signal = 0;
		cli->last_status = 130;
	}
	return (status);
}