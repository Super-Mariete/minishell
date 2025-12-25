/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:03 by rafael            #+#    #+#             */
/*   Updated: 2025/12/23 02:59:38 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_load_env(t_msh *msh, char **envp)
{
	t_env	*node;
	size_t	value_size;
	int		i;

	if (!envp || !*envp)
		return (0);
	node = msh->env;
	i = 0;
	while (envp[i] && node)
	{
		value_size = ft_strchr(envp[i], '=') - envp[i];
		if (ft_buffercpy(envp[i], msh->env_cursor, value_size + 1) )
			return (1);
		node->key = msh->env_cursor;
		msh->env_cursor += value_size + 1;
		value_size = ft_strlen(envp[i] + value_size);
		if (ft_buffercpy(envp[i] + value_size + 1, msh->env_cursor, value_size + 1))
			return (1);
		node->value = msh->env_cursor;
		msh->env_cursor += value_size + 1;
		i++;
		node = node->next;
	}
	return (0);
}

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
