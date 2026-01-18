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

static char	*create_env_str(const char *key, const char *value)
{
	char	*tmp;
	char	*newvar;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	newvar = ft_strjoin(tmp, value);
	free(tmp);
	return (newvar);
}

static int	update_existing_var(t_shenv *cur, const char *key,
				const char *value)
{
	char	*newvar;

	newvar = create_env_str(key, value);
	if (!newvar)
		return (1);
	free(cur->var);
	cur->var = newvar;
	return (0);
}

static int	add_new_var(t_shenv **ft_env, const char *key, const char *value)
{
	t_shenv	*new_node;
	char	*newvar;

	newvar = create_env_str(key, value);
	if (!newvar)
		return (1);
	new_node = malloc(sizeof(t_shenv));
	if (!new_node)
	{
		free(newvar);
		return (1);
	}
	new_node->var = newvar;
	new_node->next = *ft_env;
	*ft_env = new_node;
	return (0);
}

int	set_env(t_shenv **ft_env, char *key, const char *value)
{
	t_shenv	*cur;
	size_t	key_len;

	if (!ft_env || !key)
		return (1);
	if (!value)
		value = "";
	key_len = ft_strlen(key);
	cur = *ft_env;
	while (cur)
	{
		if (cur->var && ft_strncmp(cur->var, key, key_len) == 0
			&& cur->var[key_len] == '=')
			return (update_existing_var(cur, key, value));
		cur = cur->next;
	}
	return (add_new_var(ft_env, key, value));
}
