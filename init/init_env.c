/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:03 by rafael            #+#    #+#             */
/*   Updated: 2025/12/22 17:03:25 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	ft_load_env(t_env *env, char **envp)
// {
// 	if (!env)
// 		env->key = "PATH";
// 		env->
// }

int	ft_init_var_list(t_env *pool)
{
	t_env	*node;
	t_env	*temp;
	int		i;

	if (!pool || VAR_MAX <= 0)
		return (1);
	i = 0;
	node = pool;
	temp = pool;
	temp++;
	while (i < VAR_MAX - 1)
	{
		node->next = temp;
		i++;
		temp++;
		node = node->next;
	}
	node->next = NULL;
	return (0);
}
