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
#include "../exec.h"
/*
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
}*/

int	apply_redirs(t_cli *cli)
{
	int fd;
	if (!cli)
		return (0);
	if (cli->heredoc_fd >= 0)
	{
		if (dup2(cli->heredoc_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 heredoc");
			close(cli->heredoc_fd);
			return (1);
		}
		close(cli->heredoc_fd);
	}
	else if(cli->infile)
	{
		fd = open (cli->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cli->infile);
			return (1);
		}
		if(dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 infile");
			close(fd);
			return (1);
		}
		close(fd);
	}
	if (cli->outfile)
	{
		int flags = O_WRONLY | O_CREAT;

		if (cli->r_mode == APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(cli->outfile, flags, 0644);
		if(fd < 0)
		{
			perror(cli->outfile);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 outfile");
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
}
