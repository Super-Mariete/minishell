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

int	unset_env(t_shenv **ft_env, char *key)
{
	t_shenv *cur;
	t_shenv *prev;
	size_t	len;

	if (!ft_env || !key)
		return (0);
	len = ft_strlen(key);
	cur = *ft_env;
	prev = NULL;
	while (cur)
	{
		if(cur->var && ft_strncmp(cur->var, key, len) == 0 && cur ->var[len] == '=')
		{
			t_shenv *tofree = cur;
			if (prev)
				prev->next = cur->next;
			else
				*ft_env = cur->next;
			cur = cur->next;
			free(tofree->var);
			free(tofree);
			continue;
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
