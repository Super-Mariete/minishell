/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:40:44 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 17:03:08 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_export_var(const char *var)
{
	char *eq;

	if(!var)
		return;
	eq = ft_strchr(var, '=');
	if (eq)
	{
		write(1, "declare -x ", 11);
		write(1, var, eq - var);
		write(1, "=\"", 2);
		write(1, eq + 1, ft_strlen(eq + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, var, ft_strlen(var));
		write(1, "\n", 1);
	}
}

int	ft_export(char **args, t_shenv **env)
{
	int	i;
	char	*eq;
	char	*key;
	char	*val;
	int	ret;

	if(!args)
		return(1);
	if(!args[1])
	{
		t_shenv	*cur = *env;
		while(cur)
		{
			print_export_var(cur->var);
			cur = cur->next;
		}
		return (0);
	}
	i = 1;
	ret = 0;
	while(args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			key = ft_substr(args[i], 0, eq - args[i]);
			if(!key)
				return (1);
			val = ft_strdup(eq + 1);
			if(!val)
			{
				free(key);
				return (1);
			}
			if(ft_setenv(env, key, val) != 0)
				ret = 1;
			free(key);
			free(val);
		}
		else
		{
			if(!ft_getenv(*env, args[i]))
				if(ft_setenv(env, args[i], "") != 0)
					ret = 1;
		}
		i++;
	}
	return(ret);
}
