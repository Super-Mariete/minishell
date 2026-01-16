/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 10:09:47 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	quoted_len(const char *line, char quote)
{
	int	i;

	if (!line)
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == quote)
		{
			if (quote == '\'')
				return (i + 1);
			else if (quote == '\"')
			{
				if (line[i - 1] != '\\')
					return (i + 1);
			}
		}
		i++;
	}
	return (-1);
}

static bool	check_each_error(char **token, int i)
{
	if (token[i] && ft_strchr(OP_STR2, token[i][0])
			&& (token[i + 1] && ft_strchr(OP_STR2, token[i + 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == ')' && (token[i + 1]
			&& !ft_strchr(OP_STR, token[i + 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == '(' && i > 0
			&& (!ft_strchr(OP_STR, token[i - 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == '(' && token[i + 1]
			&& token[i + 1][0] == ')')
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && ft_strchr(OP_STR, token[i][0])
			&& !token[i + 1])
		return (perror_token(token[i], SYN_ERR), true);
	if (token[i] && ft_strchr(SEP_STR, token[i][0])
			&& token[i + 1] && ft_strchr(SEP_STR, token[i + 1][0]))
		return (perror_token(token[i + 1], SYN_ERR), true);
	return (false);
}

int	check_errors(char **token, const size_t len)
{
	int	i;

	if (!token)
		return (1);
	if (token[0] && ft_strchr(OP_STR2, token[0][0]))
		return (perror_token(token[0], SYN_ERR), 1);
	i = 0;
	while (i < len)
	{
		if (check_each_error(token, i))
			return (1);
		i++;
	}
	return (0);
}
