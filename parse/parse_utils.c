/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:23:11 by rafael            #+#    #+#             */
/*   Updated: 2026/01/04 17:07:20 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_quoted_len(const char *line, char quote)
{
	int	i;

	if (!line)
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == quote)
			return (i + 1);
		i++;
	}
	return (0);
}

size_t	ft_op_len(const char *line, int pos)
{
	if (!line || pos < 0)
		return (0);
	if (ft_isspace(line[pos]))
		return (0);
	if (ft_strchr(CONTROL_OP, line[pos]) && line[pos + 1] == line[pos] && !ft_strchr(PRTS, line[pos]))
		return (2);
	if (ft_strchr(CONTROL_OP, line[pos]))
		return (1);
	return (1);
}

void	ft_reset_buffer(t_buffer *buff)
{
	char	*t;

	t = buff->buffer;
	while (t <= buff->last)
	{
		*t = 0;
		t++;
	}
	buff->last = buff->buffer;
	buff->current = buff->last;
}
