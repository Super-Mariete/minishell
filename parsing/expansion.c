/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rms35 <rms35@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:24 by rafael-m          #+#    #+#             */
/*   Updated: 2025/09/20 18:41:21 by rms35            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	var_len(const char *var)
{
	size_t	i;
	size_t	len;

	if (!var)
		return (0);
	i = 1;
	len = ft_strlen(var);
	while (i < len)
	{
		if (var[i] == '?' || ft_strchr(NO_VAL_VAR, var[i]))
			return (i);
		i++;
	}
	return (i);
}

static char	*expand_var(char *token, const size_t start, const size_t end)
{
	char		*s;
	char		*t;
	const char	*var;

	if (!token)
		return (nullptr);
	if (end > ft_strlen(token))
		return (ft_strndup(token, end));
	s = ft_strndup(token + start, end);
	t = ft_strtrim(s, NO_VAL_VAR);
	if (!s || !t)
		return (free(s), free(t), nullptr);
	free(s);
	var = getenv(t);
	free(t);
	s = ft_strndup(token, start);
	t = ft_strjoin(s, var);
	free(s);
	s = ft_strjoin(t, token + start + end);
	free(t);
	return (s);
}

static bool	expanding_line(char *token, size_t *i, char **value)
{
	if (token[(*i)] == '\'' && *i > 0 && token[*i - 1] != '\\')
		*i += (quoted_len(token + *i, '\'') + 1);
	if (*i < ft_strlen(token) && token[(*i)] == '<' && token[*i + 1] == '<')
	{
		if (heredoc_len(token + *i) <= 0)
		{
			*value = (free(token), nullptr);
			return (true);
		}
		*i += (heredoc_len(token + *i) - 1);
	}
	return (false);
}

char	*expand_line(char *token, const t_cli *cli)
{
	size_t	i;
	char	*t;
	char	*value;

	i = 0;
	while (token && i < ft_strlen(token))
	{
		if (expanding_line(token, &i, &value))
			return (value);
		if (i < ft_strlen(token) && token[i] == '$'
			&& token[i + 1] && !ft_strchr(NO_VAL_VAR, token[i + 1]))
		{
			if (token[i + 1] == '?')
				t = expand_exit_status(cli->last_status, token, i);
			else
				t = expand_var(token, i, var_len(token + i));
			if (token != t)
				free(token);
			token = t;
		}
		i++;
	}
	t = ft_strtrim(token, " ");
	return (free(token), t);
}

char	**expand_tokens(char **tokens, size_t *len, const t_cli *cli, size_t i)
{
	char	*t;
	int		wc_len;

	if (!tokens)
		return (nullptr);
	while (i < *len)
	{
		wc_len = 0;
		if (ft_strchr(tokens[i], '*') && !ft_strchr(QUOTES, tokens[i][0]))
		{
			tokens = expand_wildcard(tokens, i, &wc_len);
			i = i + wc_len;
			*len = *len + wc_len - 1;
			continue ;
		}
		t = expand_line(tokens[i], cli);
		if (t && t[0] == '<' && t[1] == '<')
			tokens[i] = ft_strdup(t);
		else
			tokens[i] = escape_quotes(t);
		free(t);
		i++;
	}
	return (tokens);
}
