/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rms35 <rms35@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:45 by rafael-m          #+#    #+#             */
/*   Updated: 2025/09/20 15:43:11 by rms35            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_escape(char *line, int start, int end)
{
	char	*escaped;
	char	*t;
	char	*s;

	if (!line || start < 0 || end < start)
		return (nullptr);
	if (end == 0)
		return (ft_strdup(line));
	escaped = ft_esc_str(line + start + 1, ESC_CHARS1, end - start - 2);
	t = ft_strndup(line, start);
	if (!escaped)
		return (nullptr);
	s = ft_strjoin(t, escaped);
	if (!s)
		return (nullptr);
	free(escaped);
	free(t);
	t = ft_strjoin(s, line + end);
	free(s);
	return (t);
}

char	*esc_line(char *line, int i, int len)
{
	char	*esc;
	char	*t;

	if (ft_strchr(QUOTES, line[i]) && line[i] == line[i + 1])
	{
		t = ft_strndup(line, i);
		if (i > 0 && !t)
			return (perror("malloc1 : "), nullptr);
		if (!line[i + 2])
			return (t);
		esc = ft_strjoin(t, line + i + 2);
		if (esc != t)
			free(t);
		if (!esc)
			return (perror("malloc : "), nullptr);
		return (esc);
	}
	esc = ft_escape(line, i, len);
	return (esc);
}

static char	*escape_q(int *i, char **str)
{
	int		len;
	char	*esc;
	char	*s;

	s = *str;
	len = quoted_len(s + *i, s[(*i)]);
	if (len < 0)
		return (free(s), nullptr);
	esc = esc_line(s, *i, *i + len);
	if (!*esc)
		return (free(s), nullptr);
	*i += (len - 3);
	free(*str);
	return (esc);
}

char	*escape_quotes(char *line)
{
	int		i;
	char	*s;

	if (!line)
		return (nullptr);
	i = 0;
	s = ft_strdup(line);
	while (i < ft_strlen(s))
	{
		if (ft_strchr(QUOTES, s[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			s = escape_q(&i, &s);
			if (!s)
				return (nullptr);
		}
		i++;
	}
	return (s);
}

char	**tokenize(char *line, t_cli *cli)
{
	char	**tokens;

	if (!line)
		return (nullptr);
	if (check_prnts(line))
		return (nullptr);
	cli->n_tokens = num_s_tokens(line);
	tokens = token_sep(trim_spaces(line));
	if (!tokens)
		return (nullptr);
	tokens = expand_tokens(tokens, &(cli->n_tokens), cli);
	if (!tokens)
		return (free_tokens(tokens, cli->n_tokens), nullptr);
	if (check_errors(tokens, cli->n_tokens))
		return (free_tokens(tokens, cli->n_tokens), nullptr);
	return (tokens);
}
