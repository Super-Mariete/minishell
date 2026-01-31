/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/31 13:27:53 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	quoted_len(const char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == line[0])
			return (i + 1);
		i++;
	}
	write(2, "minishell: expected another ", 28);
	write(2, line, 1);
	write(2, "\n", 1);
	return (0);
}

static bool	check_each_error(char **token, const size_t i)
{
	if (token[i] && token[i][0] != 0 && ft_strchr(OP_STR2, token[i][0])
			&& (token[i + 1] && ft_strchr(OP_STR2, token[i + 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == '(' && (token[i + 1] && token[i + 1][0]
			&& ft_strchr(OP_STR2, token[i + 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == ')' && (token[i + 1]
		&& (token[i + 1][0] == '(' || (!ft_strchr(OP_STR, token[i + 1][0])
		&& !ft_strchr(REDIR_S, token[i + 1][0])))))
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] == '(' && token[i + 1]
			&& token[i + 1][0] == ')')
		return (perror_token(token[i + 1], SYN_ERR), true);
	if (token[i] && token[i][0] != 0 && ft_strchr(OP_STR2, token[i][0])
			&& !token[i + 1])
		return (perror_token(token[i], SYN_ERR), true);
	if (token[i] && token[i][0] != 0 && ft_strchr(REDIR_S, token[i][0])
		&& token[i + 1] && token[i + 1][0] != 0
		&& (ft_strchr(REDIR_S, token[i + 1][0])
		|| ft_strchr(OP_STR, token[i + 1][0])))
		return (perror_token(token[i + 1], SYN_ERR), true);
	return (false);
}

int	check_errors(char **token, const size_t len)
{
	size_t	i;

	if (!token)
		return (1);
	if (token[0] && token[0][0] && ft_strchr(OP_STR2, token[0][0]))
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
