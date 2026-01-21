/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:13:10 by made-ped          #+#    #+#             */
/*   Updated: 2025/10/31 13:31:39 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_cd(char **args, t_shenv **env)
{
	char	*path;
	char	cwd[1024];

	if (!args[1])
		path = ft_getenv(*env, "HOME");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd: getcwd:");
		return (1);
	}
	set_env(env, "PWD", cwd);
	return (0);
}
