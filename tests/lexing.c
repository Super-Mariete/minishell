/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:23:17 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 02:19:47 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static	size_t ft_skip_spaces(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	return (i);
}

static size_t	ft_token_len(const char *line)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(line);
	if (ft_strchr(CONTROL_OP, line[i]))
		return (i + ft_op_len(line, i));
	while (i < len)
	{
		if (ft_strchr(QUOTES, line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			if (ft_quoted_len(line + i, line[i]) <= 0)
				return (ft_perror_token(line[i], UNCLOSED), 0);
			i = (ft_quoted_len(line + i, line[i]) + i);
			continue ;
		}
		if (ft_strchr(METACHARS, line[i]))
			return (i);
		i++;
	}
	return (i);
}

void	ft_print_buffer(const t_buffer *buff)
{
	// char	*t;
	size_t	i;
	size_t	r;
	
	i = 0;
	r = buff->last - buff->buffer;
	while (i < r)
	{
		if (!buff->buffer[i])
		{
			if (i + 1 <= r && !buff->buffer[i + 1])
				return ;
			printf(" ");
		}
		else
			printf("%c", buff->buffer[i]);
		i++;
	}
	printf("\n");
	fflush(stdout);
	return ;
}

static int	ft_put_tokens(t_buffer *buff, t_read *rbuffer)
{
	size_t	i;
	size_t	len;
	const char	*max;

	i = 0;
	max = buff->buffer + BUF_MAX - 1;
	while (i < rbuffer->line_len)
	{
		len = ft_token_len(&(rbuffer->buffer[i]));
		if (!len)
			break ;
		if (i + len >= BUF_MAX - 1 || buff->last + len >= max)
			return (write(2, MEMOUT, sizeof(MEMOUT)), 1);
		ft_buffercpy(&(rbuffer->buffer[i]), buff->last, len);
		buff->last += len + 1;
		buff->current = buff->last;
		i += len;
		i += ft_skip_spaces(&(rbuffer->buffer[i]));
	}
	ft_print_buffer(buff);	
	return (0);
}

void	ft_parse(t_msh *msh)
{
	static t_ast	ast[MAX_NODES];
	static char		buffer[BUF_MAX];
	static t_buffer	buff;

	buff.buffer = buffer;
	buff.ast = ast;
	buff.current = buffer;
	buff.last = buffer;
	msh->buff = &buff;
	ft_put_tokens(&buff, msh->rbuffer);
	ft_reset_buffer(&buff);
	return ;
}
