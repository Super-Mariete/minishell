/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rms35 <rms35@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 21:51:50 by rms35             #+#    #+#             */
/*   Updated: 2025/09/15 21:51:51 by rms35            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exp_wc_error(const DIR *dir_stream, char *wc)
{
	if (!dir_stream && !wc)
	{
		perror("opendir && malloc : ");
		return ;
	}
	if (!dir_stream)
	{
		perror("opendir : ");
		free(wc);
		return ;
	}
	if (!wc)
	{
		perror("malloc : ");
		return ;
	}
	free(wc);
}

static int	match_wildcard(const char *s, const char *wc)
{
	size_t	i;
	size_t	j;
	size_t	ia;
	size_t	j_s;

	init_var(&i, &j, &ia, &j_s);
	while (s[i] && !(s[0] == '.' && wc[0] != '.'))
	{
		if (wc[j] == '*')
		{
			while (wc[j] == '*')
				j++;
			if (!wc[j])
				return (1);
			j_s = j;
			ia = i;
			continue ;
		}
		if ((wc[j] == s[i] && equ(&j, &i)) || (j_s && ft_js(&j_s, &ia, &i, &j)))
			continue ;
		return (0);
	}
	while (wc[j] == '*' && !(s[0] == '.' && wc[0] != '.'))
		j++;
	return (wc[j] == '\0');
}

static void	get_wildcard(char ***token, const size_t pos,\
		int *wc_len, struct dirent *dir)
{
	char	**t;

	if (*wc_len == 0)
		t = (char **)ft_add_re_ptr((void **)*token, dir->d_name, (int)pos);
	else
		t = (char **)ft_add_ptr((void **)*token, dir->d_name, (int)pos);
	ft_free_d(*token);
	*token = t;
	*wc_len = *wc_len + 1;
}

char	**expand_wildcard(char **token, const size_t pos, int *wc_len)
{
	DIR				*dir_stream;
	struct dirent	*dir;
	char			*wc;

	dir_stream = opendir(".");
	wc = ft_strdup(token[pos]);
	while (dir_stream && wc)
	{
		dir = readdir(dir_stream);
		if (!dir)
			break ;
		if (match_wildcard(dir->d_name, wc))
			get_wildcard(&token, pos, wc_len, dir);
	}
	if (*wc_len == 0)
		*wc_len = 1;
	return (exp_wc_error(dir_stream, wc), closedir(dir_stream), token);
}
