/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:45 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_escape(char *line, const size_t start, const size_t end)
{
	char	*escaped;
	char	*t;
	char	*s;

	if (!line || end < start)
		return (NULL);
	if (end == 0)
		return (ft_strdup(line));
	escaped = ft_esc_str(line + start + 1, ESC_CHARS1, end - start - 2);
	t = ft_strndup(line, start);
	if (!escaped)
		return (NULL);
	s = ft_strjoin(t, escaped);
	if (!s)
		return (NULL);
	free(escaped);
	free(t);
	t = ft_strjoin(s, line + end);
	free(s);
	return (t);
}

static char	*esc_line(char *line, const size_t i, const size_t len)
{
	char	*esc;
	char	*t;

	if (ft_strchr(QUOTES, line[i]) && line[i] == line[i + 1])
	{
		t = ft_strndup(line, i);
		if (i > 0 && !t)
			return (perror("malloc : "), NULL);
		if (!line[i + 2])
			return (t);
		esc = ft_strjoin(t, line + i + 2);
		if (esc != t)
			free(t);
		if (!esc)
			return (perror("malloc : "), NULL);
		return (esc);
	}
	esc = ft_escape(line, i, len);
	return (esc);
}

static char	*escape_q(size_t *i, char **str)
{
	size_t	len;
	char	*esc;
	char	*s;

	s = *str;
	len = quoted_len(s + *i);
	if (len == 0)
		return (free(s), NULL);
	esc = esc_line(s, *i, *i + len);
	if (!esc)
		return (free(s), NULL);
	*i += (len - 2);
	free(*str);
	return (esc);
}

char	*escape_quotes(const char *line)
{
	size_t	i;
	char	*s;

	if (!line)
		return (NULL);
	i = 0;
	s = ft_strdup(line);
	while (i < ft_strlen(s))
	{
		if (ft_strchr(QUOTES, s[i]))
		{
			s = escape_q(&i, &s);
			if (!s)
				return (NULL);
		}
		else
			i++;
	}
	return (s);
}

char	**tokenize(const char *line, t_cli *cli)
{
	char	**tokens;

	if (!line)
		return (NULL);
	if (check_prnts(line))
		return (NULL);
	cli->n_tokens = num_s_tokens(line);
	tokens = token_sep(trim_spaces(line), 0, 0);
	if (!tokens)
		return (NULL);
	tokens = expand_tokens(tokens, &(cli->n_tokens), cli, 0);
	if (!tokens)
		return (NULL);
	if (check_errors(tokens, cli->n_tokens))
		return (free_tokens(tokens, cli->n_tokens), NULL);
	return (tokens);
}
