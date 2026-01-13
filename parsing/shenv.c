/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shenv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 10:09:47 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_shenv **ft_env)
{
	t_shenv	*node;
	t_shenv	*next;

	if (!ft_env || !*ft_env)
		return ;
	node = *ft_env;
	while (node)
	{
		next = node ->next;
		free(node->var);
		free(node);
		node = next;
	}
	*ft_env = nullptr;
}

static int	shenv_len(const t_shenv *env)
{
	int	len;

	len = 0;
	if (!env)
		return (0);
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

char	**getshenv(const t_shenv *env)
{
	const t_shenv	*node;
	char			**r;
	int				len;
	int				i;

	if (!env)
		return (nullptr);
	len = shenv_len(env);
	r = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!r)
		return (perror("minishell : malloc : "), nullptr);
	i = 0;
	node = env;
	while (node)
	{
		r[i] = ft_strdup(node->var);
		if (node->var && !r[i])
			return (perror("minishell : malloc : "), nullptr);
		i++;
		node = node->next;
	}
	r[i] = nullptr;
	return (r);
}

t_shenv	*load_env(char **envp)
{
	int		i;
	t_shenv	*env;
	t_shenv	*node;

	if (!envp)
		return (nullptr);
	env = (t_shenv *)ft_calloc(1, sizeof(t_shenv));
	if (!env)
		return (perror("minishell : malloc : "), nullptr);
	i = 0;
	node = env;
	while (envp[i])
	{
		node->var = ft_strdup(envp[i]);
		if (!node->var && envp[i] && envp[i][0])
			return (free_env(&env), perror("minishell : malloc : "), nullptr);
		if (!envp[++i])
			break ;
		node->next = (t_shenv *)ft_calloc(1, sizeof(t_shenv));
		if (!node->next)
			return (free_env(&env), perror("minishell : malloc : "), nullptr);
		node = node->next;
	}
	return (node->next = nullptr, env);
}
