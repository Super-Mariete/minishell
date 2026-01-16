/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:26:32 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 16:35:16 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	unset_var(t_shenv **env, t_shenv **cur, t_shenv *prev)
{
	t_shenv	*tofree;

	tofree = *cur;
	if (prev)
		prev->next = (*cur)->next;
	else
		*env = (*cur)->next;
	*cur = (*cur)->next;
	free(tofree->var);
	free(tofree);
}

int	unset_env(t_shenv **env, char *key)
{
	t_shenv	*cur;
	t_shenv	*prev;
	size_t	len;

	if (!env || !key)
		return (0);
	len = ft_strlen(key);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (cur->var && ft_strncmp(cur->var,
				key, len) == 0 && cur ->var[len] == '=')
		{
			unset_var(env, &cur, prev);
			continue ;
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
