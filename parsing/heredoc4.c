/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 13:12:50 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/19 13:20:17 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*convert_to_string(char **array)
{
	char	*s;
	char	*str;
	char	*nl;
	size_t	i;

	i = 0;
	str = NULL;
	if (!array)
		return (NULL);
	while (array[i])
	{
		s = ft_strjoin(str, array[i]);
		if ((str || array[i]) && !s)
			return (free(str), free(array[i]), perror("msh: malloc:"), NULL);
		nl = ft_strjoin(s, "\n");
		free(str);
		free(s);
		if (!nl)
			return (perror("minishell : malloc:"), NULL);
		str = nl;
		free(array[i]);
		i++;
	}
	free(array);
	return (str);
}

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
		if (!herearray && errno == ENOMEM)
			return (ft_free_d(herearray), NULL);
		t = convert_to_string(herearray);
		free(cli->heredoc);
		cli->heredoc = t;
	}
	return (cli->heredoc);
}

int	handle_heredoc(t_cli *cli, const int *option,\
		const char *delim, const char *line)
{
	if (!line)
		here_error(delim);
	cli->heredoc = expand_heredoc(*option, cli);
	if (!cli->heredoc && errno == ENOMEM)
	{
		perror("minishell: malloc:");
		cli->status = 2;
		return (2);
	}
	if (!cli->heredoc)
		cli->heredoc = ft_strdup("\0");
	return (0);
}
