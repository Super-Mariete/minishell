/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:59:14 by made-ped          #+#    #+#             */
/*   Updated: 2026/01/05 18:56:21 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	redir_heredoc(const t_cli *cli)
{
	if (dup2(cli->heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 heredoc");
		close(cli->heredoc_fd);
		return (true);
	}
	close(cli->heredoc_fd);
	return (false);
}

static bool	redir_infile(const t_cli *cli, int *fd)
{
	*fd = open (cli->infile, O_RDONLY);
	if (*fd < 0)
	{
		perror(cli->infile);
		return (true);
	}
	if (dup2(*fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 infile");
		close(*fd);
		return (true);
	}
	close(*fd);
	return (false);
}

static bool	redir_outfile(const t_cli *cli, int *fd)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cli->r_mode == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	*fd = open(cli->outfile, flags, 0644);
	if (*fd < 0)
	{
		perror(cli->outfile);
		return (true);
	}
	if (dup2(*fd, STDOUT_FILENO) < 0)
	{
		perror("minishel: dup2 outfile");
		close(*fd);
		return (true);
	}
	close(*fd);
	return (false);
}

int	apply_redirs(const t_cli *cli)
{
	int	fd;

	if (!cli)
		return (0);
	if (cli->heredoc_fd >= 0)
	{
		if (redir_heredoc(cli))
			return (1);
	}
	else if (cli->infile)
	{
		if (redir_infile(cli, &fd))
			return (1);
	}
	if (cli->outfile)
	{
		if (redir_outfile(cli, &fd))
			return (1);
	}
	return (0);
}
