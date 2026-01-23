/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 10:09:47 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	expand_t(char ***tokens, size_t *len, size_t *i, int wc_len)
{
	if (!ft_strchr(QUOTES, (*tokens)[(*i)][0])
			&& ft_strchr((*tokens)[(*i)], '*'))
	{
		wc_len = 0;
		*tokens = expand_wildcard(*tokens, *i, &wc_len);
		*i += wc_len;
		*len += wc_len - 1;
		return (true);
	}
	if ((*tokens)[(*i)][0] == '<' && (*tokens)[(*i)][1] == '<')
	{
		*i += 2;
		return (true);
	}
	return (false);
}

char	*expand_exit_status(const int status, const char *line, const size_t i)
{
	char	*before;
	char	*after;
	char	*status_str;
	char	*tmp;
	char	*new_line;

	before = ft_strndup(line, i);
	after = ft_strdup(line + i + 2);
	status_str = ft_itoa(status);
	tmp = ft_strjoin(before, status_str);
	new_line = ft_strjoin(tmp, after);
	free(before);
	free(after);
	free(status_str);
	free(tmp);
	return (new_line);
}

void	reset_free(t_cli *cli)
{
	t_cli	*node;
	t_cli	*next_node;

	if (!cli)
		return ;
	node = cli;
	while (node->prev)
		node = node->prev;
	while (node)
	{
		next_node = node->next;
		free_node(node);
		node = next_node;
	}
}

size_t	n_quotes(const char *line)
{
	size_t	i;
	size_t	n;

	if (!line)
		return (SIZE_MAX);
	i = 0;
	n = 0;
	while (line[i])
	{
		if (ft_strchr(QUOTES, line[i]))
			n++;
		i++;
	}
	return (n);
}
