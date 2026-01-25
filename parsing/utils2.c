/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_list(t_cli *cli)
{
	t_cli		*node;
	t_cli		*next_node;

	if (!cli)
		return ;
	node = cli;
	free_env(node->env);
	while (node && node->prev)
		node = node->prev;
	while (node)
	{
		next_node = node->next;
		free_node(node);
		node = next_node;
	}
}

void	free_node(t_cli *cli)
{
	if (!cli)
		return ;
	free(cli->cmd);
	cli->cmd = NULL;
	free(cli->heredoc);
	cli->heredoc = NULL;
	free(cli->infile);
	cli->infile = NULL;
	free(cli->outfile);
	cli->outfile = NULL;
	ft_free_d(cli->args);
	cli->args = NULL;
	if (cli->heredoc_fd != -1)
		close(cli->heredoc_fd);
	cli->heredoc_fd = -1;
	free(cli);
}

static size_t	trim_s_len(const char *line)
{
	size_t	i;
	size_t	qlen;
	size_t	len;

	i = 0;
	len = 0;
	while (line && i < ft_strlen(line))
	{
		if (ft_strchr(QUOTES, line[i]))
		{
			qlen = quoted_len(line + i);
			if (!qlen)
				return (0);
			len += qlen;
			i += qlen;
			continue ;
		}
		while (ft_isspace(line[i]) && ((i + 1)
				>= ft_strlen(line) || ft_isspace(line[i + 1])))
			i++;
		i++;
		len++;
	}
	return (len);
}

void	skip_spaces(const char *line, int *i)
{
	while (ft_isspace(line[(*i)]) && (ft_isspace(line[*i + 1])
			|| !line[*i + 1]))
		(*i)++;
}

char	*trim_spaces(const char *line)
{
	int		i;
	int		j;
	char	sep;
	char	*trimmed;

	i = 0;
	if (trim_s_len(line) == 0)
		return (NULL);
	trimmed = ft_calloc(trim_s_len(line) + 1, sizeof(char));
	j = 0;
	while (trimmed && i < (int)ft_strlen(line))
	{
		skip_spaces(line, &i);
		if (i < (int)ft_strlen(line) && ft_strchr(QUOTES,
				line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			sep = line[i];
			trimmed[j++] = line[i++];
			while (i < (int)ft_strlen(line) && line[i] != sep)
				trimmed[j++] = line[i++];
		}
		if (i < (int)ft_strlen(line))
			trimmed[j++] = line[i++];
	}
	return (trimmed);
}
