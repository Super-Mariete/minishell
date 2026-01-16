/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 17:04:02 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 17:06:43 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_unset(char **args, t_shenv **ft_env)
{
	int	i;

	(void)ft_env;
	if (!args)
		return (1);
	i = 1;
	while (args[i])
	{
		unset_env(ft_env, args[i]);
		i++;
	}
	return (0);
}
