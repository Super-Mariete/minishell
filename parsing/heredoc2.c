/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 11:46:52 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/12 11:56:37 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int write_to_heredoc(const t_cli *cli, char file[10], const int fd)
{
	int	ret;

	write(fd, cli->heredoc, ft_strlen(cli->heredoc));
	close(fd);
	ret = open(file, O_RDONLY, 0444);
	if (ret == -1)
	{
		perror_msh("open", nullptr);
		perror(nullptr);
	}
	unlink(file);
	return (ret);
}

int	create_heredoc(const t_cli *cli)
{
	int		fd;
	ssize_t	i;
	char	file[10];

	i = 0;
	fd = open("/dev/urandom", O_RDWR | O_EXCL, 0644);
	if (fd == -1)
	{
		while (i++ < 10)
			file[i - 1] = (char)i;
	}
	else
	{
		if (read(fd, file, 9) <= 0)
			while (i++ < 10)
				file[i - 1] = (char)i;
	}
	fd = open(file, O_RDWR | O_CREAT, 0644);
	if (fd == -1)
	{
		perror_msh("open", nullptr);
		perror(nullptr);
		return (-1);
	}
	write_to_heredoc(cli, file, fd);
	return (fd);
}