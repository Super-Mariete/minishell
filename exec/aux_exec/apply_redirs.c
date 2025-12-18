/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:59:14 by made-ped          #+#    #+#             */
/*   Updated: 2025/12/18 12:15:20 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec.h"

int	apply_redirs(t_cli *cli)
{
	int	fd;

	if (!cli)
		return (0);
	if(cli->infile)
	{
		fd = open(cli->infile, O_RDONLY);
		if (fd < 0)
			return (perror(cli->infile), 1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cli->heredoc_fd != -1)
		dup2(cli->heredoc_fd, STDIN_FILENO);
	if (cli->outfile)
	{
		fd = open(cli->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(cli->outfile), 1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}
