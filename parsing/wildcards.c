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

void	exp_wc_error(DIR *dir_stream, char *wc)
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
	return ;
}

char	**expand_wildcard(char **token, int pos, int *wc_len)
{
	DIR				*dir_stream;
	struct dirent	*dir;
	char	*wc;
	char	**t;

	dir_stream = opendir(".");
	wc = ft_strdup(token[pos]);
	while (dir_stream && wc)
	{
		dir = readdir(dir_stream);
		if (!dir)
			break ;
		if (match_wildcard(dir->d_name, wc))
		{
			if (*wc_len == 0)
				t = (char **)ft_add_re_ptr((void **)token, dir->d_name, pos);
			else
				t = (char **)ft_add_ptr((void **)token, dir->d_name, pos);
			ft_free_d(token);
			token = t;
			*wc_len = *wc_len + 1;
		}
	}
	if (*wc_len == 0)
		*wc_len = 1;
	return (exp_wc_error(dir_stream, wc), closedir(dir_stream), token);
}

int	match_wildcard(const char *str, const char *wildcard)
{
	size_t	i;
	size_t	j;
	size_t	i_a;
	size_t	j_s;

	init_var(&i, &j, &i_a, &j_s);
    while (str[i] && !(str[0] == '.' && wildcard[0] != '.'))
    {
        if (wildcard[j] == '*')
        {
			while (wildcard[j] == '*')
				j++;
			if (!wildcard[j])
				return (1);
			j_s = j;
			i_a = i;
			continue ;
        }
        if ((wildcard[j] == str[i] && ft_equal(&j, &i)) || (j_s && ft_j_s(&j_s, &i_a, &i, &j)))
			continue ;
		return (0);
    }
    while (wildcard[j] == '*' && !(str[0] == '.' && wildcard[0] != '.'))
		j++;
    return (wildcard[j] == '\0');
}