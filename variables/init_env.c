/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:03 by rafael            #+#    #+#             */
/*   Updated: 2025/12/29 18:52:16 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_load_env(t_env *env, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	while (envp[i])
	{
		len = ft_strlen(envp[i]);
		if (env->cursor + len - env->arena >= ARG_MAX)
			return (write(2, ARG2BIG, ft_strlen(ARG2BIG)), 126);
		*(env->cursor) = (uint8_t)2;
		env->cursor++;
		if (ft_buffercpy(envp[i], env->cursor, len))
			return (write(2, ARG2BIG, ft_strlen(ARG2BIG)), 126);
		if (!env->head)
			env->head = (env->cursor - 1);
		env->last = (env->cursor - 1);
		env->cursor += (len + 1);
		i++;
	}
	return (0);
}
