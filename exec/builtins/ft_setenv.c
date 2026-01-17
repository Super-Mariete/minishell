/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:01:28 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 16:24:53 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	get_var(const char *value, t_shenv *cur, char *newvar, char **tmp)
{
	*tmp = ft_strjoin(*tmp, value);
	free(*tmp);
	if (!newvar)
		return (1);
	free(cur->var);
	cur->var = newvar;
	return (0);
}

static bool	set_var(const char *key, const char *value, t_shenv **cur)
{
	char	*newvar;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (true);
	newvar = ft_strjoin(tmp, value);
	free(tmp);
	if (!*newvar)
		return (true);
	*cur = malloc(sizeof(t_shenv));
	if (!*cur)
	{
		free(newvar);
		return (true);
	}
	return (false);
}

int	set_env(t_shenv **ft_env, char *key, const char *value)
{
	t_shenv	*cur;
	char	*newvar;
	char	*tmp;

	if (!ft_env || !key)
		return (1);
	if (!value)
		value = "";
	tmp = nullptr;
	newvar = nullptr;
	cur = *ft_env;
	while (cur)
	{
		if (cur->var && ft_strncmp(cur->var, key, ft_strlen(key)) == 0
			&& cur->var[ft_strlen(key)] == '=')
			return (get_var(value, cur, newvar, &tmp));
		cur = cur->next;
	}
	if (set_var(key, value, &cur))
		return (1);
	cur->var = newvar;
	cur->next = *ft_env;
	*ft_env = cur;
	return (0);
}
