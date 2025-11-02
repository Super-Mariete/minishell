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

char	*ft_getenv(t_shenv *env, char *key)
{
	size_t	len;

	if(!key)
		return (NULL);
	len = ft_strlen(key);
	while(env)
	{
		if(env->var && ft_strncmp(env->var, key, len) == 0 && env->var[len] == '=')
			return (env->var + len + 1);
		env = env->next;
	}
	return (NULL);
}
