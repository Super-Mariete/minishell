/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:23:17 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 15:19:17 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	ft_skip_spaces(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	return (i);
}

// If want to escape quotes, add: && (i == 0 || (i > 0 && line[i - 1] != '\\'))
static size_t	ft_token_len(const char *line)
{
	size_t	i;
	size_t	line_len;
	size_t	qlen;

	i = 0;
	line_len = ft_strlen(line);
	if (ft_strchr(CONTROL_OP, line[i]))
		return (ft_op_len(line, i));
	while (i < line_len)
	{
		if (ft_strchr(QUOTES, line[i]))
		{
			qlen = ft_quoted_len(line + i, line[i]);
			if (qlen == 0)
				return (ft_perror_token(line[i], UNCLOSED), 0);
			i = (qlen + i);
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
	size_t	i;
	size_t	len;
	size_t	printed;

	i = 0;
	printed = 0;
	len = buff->last - buff->buffer;
	while (i < len)
	{
		if (!buff->buffer[i])
			write(2, " ", 1);
		else
		{
			write(2, &(buff->buffer[i]), 1);
			printed++;
		}
		i++;
	}
	if (printed)
		write(2, "\n", 1);
	return ;
}

static int	ft_put_tokens(t_buffer *buff, t_read *rbuffer)
{
	size_t		i;
	size_t		len;
	const char	*max;

	i = 0;
	max = buff->buffer + BUF_MAX - 1;
	while (i < rbuffer->line_len)
	{
		i += ft_skip_spaces(&(rbuffer->buffer[i]));
		if (!rbuffer->buffer[i])
			return (0);
		len = ft_token_len(&(rbuffer->buffer[i]));
		if (len == 0)
			break ;
		if (i + len >= BUF_MAX - 1 || buff->last + len >= max)
			return (write(2, MEMOUT, sizeof(MEMOUT)), E2BIG);
		ft_buffercpy(&(rbuffer->buffer[i]), buff->last, len);
		buff->last += len + 1;
		buff->current = buff->last;
		i += len;
	}
	ft_print_buffer(buff);
	return (0);
}

static void	ft_init_ast(t_msh *msh)
{
	static t_node	node[MAX_NODES + 1];
	static t_ast	ast;

	ast.first = node;
	ast.current = node;
	ast.n_nodes = 1;
	msh->ast = &ast;
	return ;
}

size_t	ft_lexer(t_msh *msh)
{
	static char		buffer[BUF_MAX];
	static t_buffer	buff;
	int				status;

	buff.buffer = buffer;
	ft_init_ast(msh);
	buff.current = buffer;
	buff.last = buffer;
	msh->buff = &buff;
	status = ft_put_tokens(&buff, msh->rbuffer);
	if (status)
		*msh->status = status;
	ft_reset_read(msh->rbuffer);
	return (0);
}
