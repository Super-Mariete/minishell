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

char	*trim_delim(const char *token, int *option)
{
	char	*delim;
	int		i;
	int		j;
	size_t	quotes;
	size_t	len;

	quotes = n_quotes(token);
	len = ft_strlen(token);
	if (quotes == SIZE_MAX || len - quotes == 0)
		return (ft_strdup("\0"));
	if (ft_strchr(token, '\"') || ft_strchr(token, '\''))
		*option = 1;
	delim = (char *)ft_calloc(len - quotes + 1, sizeof(char));
	if (!delim)
		return (perror("minishell: malloc:"), NULL);
	i = 0;
	j = 0;
	while (token[i])
	{
		if (!ft_strchr(QUOTES, token[i]))
			delim[j++] = token[i];
		i++;
	}
	return (delim);
}

size_t	heredoc_len(const char *line)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (i < ft_strlen(line) && i < 2 && ft_strchr(REDIR_S, line[i]))
		i++;
	while (line[i] && ft_isspace(line[i]))
		i++;
	while (i < ft_strlen(line) && line[i])
	{
		if (i < ft_strlen(line) && ft_strchr(QUOTES, line[i]))
		{
			len = quoted_len(line + i);
			if (len == 0)
				return (0);
			i += (len + 1);
			continue ;
		}
		if (ft_strchr(SEP_STR, line [i]))
			return (i);
		i++;
	}
	return (i);
}

static int	write_to_heredoc(t_cli *cli, const char *file, const int fd)
{
	int	ret;

	write(fd, cli->heredoc, ft_strlen(cli->heredoc));
	close(fd);
	free(cli->heredoc);
	cli->heredoc = NULL;
	ret = open(file, O_RDONLY, 0444);
	if (ret == -1)
	{
		perror_msh("open", nullptr);
		perror(nullptr);
	}
	unlink(file);
	return (ret);
}

static void	generate_random_filename(char *buffer)
{
	int		fd;
	int		i;
	char	rand_bytes[10];

	ft_strlcpy(buffer, ".heredoc_", 30);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0 || read(fd, rand_bytes, 10) < 10)
	{
		i = 0;
		while (i < 10)
		{
			rand_bytes[i] = (char)(i * 3);
			i++;
		}
	}
	if (fd >= 0)
		close(fd);
	i = 0;
	while (i < 10)
	{
		buffer[9 + i] = "abcdefghijklmnopqrstuvwxyz0123456789"
		[(unsigned char)rand_bytes[i] % 36];
		i++;
	}
	buffer[9 + i] = '\0';
}

int	create_heredoc(t_cli *cli)
{
	int		fd;
	char	file[30];

	if (cli->heredoc_fd != -1)
		close(cli->heredoc_fd);
	generate_random_filename(file);
	fd = open(file, O_RDWR | O_CREAT | O_EXCL, 0644);
	if (fd == -1)
	{
		perror_msh("open", nullptr);
		return (perror(nullptr), -1);
	}
	return (write_to_heredoc(cli, file, fd));
}
