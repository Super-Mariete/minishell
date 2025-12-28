/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:03 by rafael            #+#    #+#             */
/*   Updated: 2025/12/28 01:40:38 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	ft_get_key(t_msh *msh, const char *env, t_env *node)
{
	size_t	key_size;

	key_size = ft_strchr(env, '=') - env;
	if ((msh->env_cursor + key_size) - msh->env_arena >= ARG_MAX)
	{
		write(2, "minishell: Argument list too long\n", 34);
		return (0);
	}
	if (ft_buffercpy(env, msh->env_cursor, key_size))
		return (0);
	node->key = msh->env_cursor;
	msh->env_cursor += key_size + 1;
	return (key_size);
}

static int	ft_get_value(t_msh *msh, const char *env, t_env *node)
{
	size_t	value_size;

	value_size = ft_strlen(env);
	if ((msh->env_cursor + value_size) - msh->env_arena >= ARG_MAX)
	{
		write(2, "minishell: Argument list too long\n", 34);
		return (0);
	}
	if (ft_buffercpy(env, msh->env_cursor, value_size))
		return (0);
	node->value = msh->env_cursor;
	msh->env_cursor += value_size + 1;
	return (value_size);
}

int	ft_load_env(t_msh *msh, char **envp)
{
	t_env	*node;
	size_t	value_size;
	size_t	key_size;
	int		i;

	if (!envp || !*envp)
		return (0);
	node = msh->env;
	i = 0;
	while (envp[i])
	{
		if (i >= VAR_MAX)
			return (write(2, "minishell: Argument list too long\n", 34), 126);
		key_size = ft_get_key(msh, envp[i], node);
		if (!key_size)
			return (126);
		value_size = ft_get_value(msh, envp[i] + key_size + 1, node);
		if (!value_size)
			return (126);
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
