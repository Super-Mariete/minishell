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

void	ft_exec(t_cli *cli)
{
	if (cli->status != 130)
	{
		cli->status = execute(cli);
		cli->last_status = cli->status;
	}
	reset_list(cli);
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
