/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:55 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 12:55:01 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirs(char **token, const int i)
{
	char	*s;

	if (token[i] && token[i][0] && ft_strchr(REDIR_S, token[i][0])
			&& (!token[i][1] || (token[i][1] == token[i][0] && !token[i][2])))
	{
		if (!token[i + 1])
			return (perror_token("newline", SYN_ERR), 1);
		s = ft_strndup(token[i], 3);
		perror_token(s, SYN_ERR);
		free(s);
		return (1);
	}
	return (0);
}

static int	sep_len(const char *line, const size_t pos)
{
	if (!line)
		return (-1);
	if (ft_isspace(line[pos]))
		return (0);
	if (ft_strchr(SEP_STR, line[pos]) && line[pos + 1] == line[pos]
		&& !ft_strchr(PRNTS, line[pos]))
		return (2);
	if (ft_strchr(SEP_STR, line[pos]))
		return (1);
	return (1);
}

static int	token_len(const char *line)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(line);
	while (i < len && ft_isspace(line[i]))
		i++;
	if (ft_strchr(SEP_STR, line[i]))
		return ((int)i + sep_len(line, i));
	while (i < len)
	{
		if (ft_strchr(QUOTES, line[i]) && (i == 0 || (line[i - 1] != '\\')))
		{
			if (quoted_len(line + i) == 0)
				return (-1);
			i = (quoted_len(line + i) + i);
			continue ;
		}
		if (ft_strchr(SEP_STR, line[i]))
			return ((int)i);
		i++;
	}
	return ((int)i);
}

int	num_s_tokens(const char *line)
{
	size_t	i;
	int		len;
	size_t	line_len;
	int		num_token;

	i = 0;
	num_token = 0;
	line_len = ft_strlen(line);
	while (i < line_len)
	{
		if (ft_isspace(line[i]) && (ft_isspace(line[i + 1]) || !line[i + 1]))
		{
			i++;
			continue ;
		}
		len = token_len(line + i);
		if (len <= 0)
			return (-1);
		i += len;
		num_token++;
	}
	return (num_token);
}

char	**token_sep(char *line, size_t j, size_t i)
{
	int		len;
	char	**tokens;

	if (!line)
		return (nullptr);
	len = num_s_tokens(line);
	if (len <= 0)
		return (free(line), nullptr);
	tokens = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!tokens)
		return (free(line), perror("minishell: malloc : "), nullptr);
	i = 0;
	j = 0;
	while (i < ft_strlen(line))
	{
		while (ft_isspace(line[i]))
			i++;
		if (i >= ft_strlen(line))
			break ;
		if (token_len(line + i) <= 0)
			return (tokens[len] = NULL, ft_free_d(tokens), free(line), NULL);
		tokens[j++] = ft_strndup(line + i, token_len(line + i));
		i += token_len(line + i);
	}
	return (free(line), tokens[len] = nullptr, tokens);
}
