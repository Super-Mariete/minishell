/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:01:28 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 16:24:53 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_setenv(t_shenv **env, char *key, char *value)
{
	t_shenv *cur;
	char	*newvar;
	char	*tmp;

	if (!env || !key)
		return (1);
	if (!value)
		value = "";

	cur = *env;
	while(cur)
	{
		if(cur->var && ft_strncmp(cur->var, key, ft_strlen(key)) == 0
			&& cur->var[ft_strlen(key)] == '=')
		{
			tmp = ft_strjoin(tmp, value);
			free(tmp);
			if(!newvar)
				return(1);
			free(cur->var);
			cur->var = newvar;
			return(0);
		}
		cur = cur->next;
	}
	tmp = ft_strjoin(key, "=");
	if(!tmp)
		return (1);
	newvar = ft_strjoin(tmp, value);
	free(tmp);
	if (!newvar)
		return (1);
	cur = malloc(sizeof(t_shenv));
	if(!cur)
	{
		free(newvar);
		return (1);
	}
	cur->var = newvar;
	cur->next = *env;
	*env = cur;
	return (0);
}
