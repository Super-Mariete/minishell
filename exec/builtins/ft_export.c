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
	char	*eq;

	if (!var)
		return ;
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

static int	export_error(t_shenv **env, char **args)
{
	const t_shenv	*cur;

	if (!args)
		return (1);
	cur = *env;
	while (cur)
	{
		print_export_var(cur->var);
		cur = cur->next;
	}
	return (0);
}

static bool	export_arg(const char *arg, t_shenv **env, const char *eq, int *ret)
{
	char	*key;
	char	*val;

	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (true);
	val = ft_strdup(eq + 1);
	if (!val)
	{
		free(key);
		return (true);
	}
	if (set_env(env, key, val) != 0)
		*ret = 1;
	free(key);
	free(val);
	return (false);
}

int	ft_export(char **args, t_shenv **env)
{
	int			i;
	const char	*eq;
	int			ret;

	if (!args || !args[1])
		return (export_error(env, args));
	i = 1;
	ret = 0;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			if (export_arg(args[i], env, eq, &ret))
				return (1);
		}
		else
		{
			if (!ft_getenv(*env, args[i]))
				if (set_env(env, args[i], "") != 0)
					ret = 1;
		}
		i++;
	}
	return (ret);
}
