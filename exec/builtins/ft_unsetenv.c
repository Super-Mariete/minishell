/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:26:32 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 16:35:16 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_unsetenv(t_shenv **env, char *key)
{
	t_shenv *cur;
	t_shenv *prev;
	size_t	len;

	if (!env || !key)
		return (0);
	len = ft_strlen(key);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if(cur->var && ft_strncmp(cur->var, key, len) == 0 && cur ->var[len] == '=')
		{
			t_shenv *tofree = cur;
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
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
