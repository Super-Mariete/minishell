/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 11:02:33 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/18 11:02:33 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	s_len(const char *str, const int n)
{
	size_t	i;

	i = 0;
	if (n)
	{
		while (!ft_strchr(QUOTES, str[i]))
			i++;
		return (i);
	}
	while (ft_strchr(QUOTES, str[i]))
		i++;
	return (i);
}

static char	*end_quotes(const char *quoted,
		const char *str, const size_t len)
{
	char	*end;
	char	*s;

	end = ft_strdup(quoted + len);
	s = ft_strjoin(str, end);
	if (!end || !s)
		return (free(end), NULL);
	free(end);
	return (s);
}

static char	*reapply_quotes(const char *quoted, const char *str)
{
	char	*begin;
	char	*t;
	char	*ret;
	size_t	len;

	len = s_len(quoted, 0);
	ret = NULL;
	if (len)
	{
		begin = ft_strndup(quoted, len);
		ret = ft_strjoin(begin, str);
		if (!begin || !ret)
			return (free(begin), NULL);
		free(begin);
	}
	if (s_len(quoted + len, 1))
	{
		if (ret)
			t = end_quotes(quoted, ret, len + s_len(quoted + len, 1));
		else
			t = end_quotes(quoted, (char *)str, len + s_len(quoted + len, 1));
		free(ret);
		ret = t;
	}
	return (ret);
}

char	**expand_array(char **array, const t_cli *cli)
{
	size_t	i;
	char	*expanded;
	char	*t;

	i = 0;
	while (array[i])
	{
		t = ft_strtrim(array[i], QUOTES);
		if (t && t[0] == '$')
		{
			expanded = expand_line(t, cli);
			free(t);
			t = reapply_quotes(array[i], expanded);
			if (!t)
				return (NULL);
			free(array[i]);
			free(expanded);
			array[i] = t;
		}
		else
			free(t);
		i++;
	}
	return (array);
}

char	*convert_to_string(char **array)
{
	char	*s;
	char	*str;
	char	*nl;
	size_t	i;

	i = 0;
	str = NULL;
	while (array[i])
	{
		s = ft_strjoin(str, array[i]);
		nl = ft_strjoin(s, "\n");
		free(str);
		free(s);
		str = nl;
		free(array[i]);
		i++;
	}
	free(array);
	return (str);
}
