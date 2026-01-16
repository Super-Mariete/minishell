/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 10:09:47 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_list(t_cli **cli)
{
	t_cli		*node;
	t_cli		*next_node;

	if (!cli || !*cli)
		return ;
	node = *cli;
	while (node)
	{
		next_node = node->next;
		free(node->cmd);
		node->cmd = nullptr;
		free(node->heredoc);
		node->heredoc = nullptr;
		free(node->infile);
		node->infile = nullptr;
		free(node->outfile);
		node->outfile = nullptr;
		ft_free_d(node->args);
		node->args = nullptr;
		free(node);
		node = next_node;
	}
	*cli = nullptr;
}

void	free_node(t_cli *cli)
{
	if (!cli)
		return ;
	free(cli->cmd);
	cli->cmd = nullptr;
	free(cli->heredoc);
	cli->heredoc = nullptr;
	free(cli->infile);
	cli->infile = nullptr;
	free(cli->outfile);
	cli->outfile = nullptr;
	ft_free_d(cli->args);
	cli->args = nullptr;
	free(cli);
}

static int	trim_s_len(const char *line)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (line && i < ft_strlen(line))
	{
		if (ft_strchr(QUOTES,
				line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			if (quoted_len(line + i, line[i]) <= 0)
				return (-1);
			len += quoted_len(line + i, line[i]);
			i += quoted_len(line + i, line[i]);
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
	if (trim_s_len(line) < 0)
		return (nullptr);
	trimmed = ft_calloc(trim_s_len(line) + 1, sizeof(char));
	j = 0;
	while (trimmed && line && i < ft_strlen(line))
	{
		skip_spaces(line, &i);
		if (i < ft_strlen(line) && ft_strchr(QUOTES,
				line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			sep = line[i];
			trimmed[j++] = line[i++];
			while (i < ft_strlen(line) && line[i] != sep)
				trimmed[j++] = line[i++];
		}
		if (i < ft_strlen(line))
			trimmed[j++] = line[i++];
	}
	return (trimmed);
}
