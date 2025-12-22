/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:03 by rafael            #+#    #+#             */
/*   Updated: 2025/12/22 21:01:04 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_load_env(t_env *env, char *var_pool, char **envp)
{
	char	*head;
	t_env	*node;
	char	*len;
	int		i;
	
	head = var_pool;
	if (!env)
	{
		env->key = ft_strlcpy(var_pool, "PATH", 5);
		env->value = ft_strlcpy(var_pool, PATH, ft_strlen(PATH) + 1);
		head += 5 + ft_strlen(PATH) + 1;
		env->pool_head = head;
		return (0);
	}
	node = env;
	i = 0;
	while (envp[i])
	{
		len = ft_strchr(envp[i], "=") - envp[i];
		
	}
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
