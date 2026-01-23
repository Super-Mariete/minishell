/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rms35 <rms35@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:24 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/18 12:00:00 by gemini           ###   ########.fr       */
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

static char	*expand_var(char *token,
		const size_t start, const size_t end, t_shenv **env)
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
	var = ft_getenv(*env, t);
	free(t);
	s = ft_strndup(token, start);
	t = ft_strjoin(s, var);
	free(s);
	s = ft_strjoin(t, token + start + end);
	free(t);
	return (s);
}

static char	*expand_token(char *token, const t_cli *cli, size_t i)
{
	char	*t;

	if (token[i + 1] == '?')
		t = expand_exit_status(cli->status, token, i);
	else
		t = expand_var(token, i, var_len(token + i), cli->env);
	return (t);
}

char	*expand_line(char *token, const t_cli *cli)
{
	size_t	i;
	int		q;
	char	*t;

	i = 0;
	q = 0;
	while (token && i < ft_strlen(token))
	{
		if (token[i] == '\'' && q != 2)
			q = (q == 0);
		else if (token[i] == '\"' && q != 1)
			q = (q == 0) * 2;
		else if (token[i] == '$' && q != 1 && token[i + 1]
			&& !ft_strchr(NO_VAL_VAR, token[i + 1]))
		{
			t = expand_token(token, cli, i);
			free(token);
			token = t;
			if (!token)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (t = ft_strtrim(token, " "), free(token), t);
}

char	**expand_tokens(char **tokens, size_t *len, const t_cli *cli, size_t i)
{
	char	*t;
	int		wc_len;

	if (!tokens)
		return (nullptr);
	wc_len = 0;
	while (i < *len)
	{
		if (expand_t(&tokens, len, &i, wc_len))
			continue ;
		t = expand_line(tokens[i], cli);
		tokens[i] = escape_quotes(t);
		free(t);
		i++;
	}
	return (tokens);
}
