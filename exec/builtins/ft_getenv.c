/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:37:14 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 15:55:43 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(const t_shenv *ft_env, char *key)
{
	size_t	len;

	if (!key)
		return (nullptr);
	len = ft_strlen(key);
	while (ft_env)
	{
		if (ft_env->var && ft_strncmp(ft_env->var,
				key, len) == 0 && ft_env->var[len] == '=')
			return (ft_env->var + len + 1);
		ft_env = ft_env->next;
	}
	return (nullptr);
}
