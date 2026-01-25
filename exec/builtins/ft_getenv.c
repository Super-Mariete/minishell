/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:37:14 by made-ped          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(const t_shenv *ft_env, char *key)
{
	size_t	len;

	if (!key)
		return (NULL);
	len = ft_strlen(key);
	while (ft_env)
	{
		if (ft_env->var && ft_strncmp(ft_env->var,
				key, len) == 0 && ft_env->var[len] == '=')
			return (ft_env->var + len + 1);
		ft_env = ft_env->next;
	}
	return (NULL);
}
