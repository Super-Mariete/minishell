/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 17:08:27 by made-ped          #+#    #+#             */
/*   Updated: 2025/11/02 17:26:28 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_number_str(const char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	if (s[0] == '+' || s[0] == '-')
		i++;
	if (!s[i])
		return (false);
	while (s[i])
	{
		if (!ft_isdigit((int)s[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	ft_exit2(char **args)
{
	int		exitcode;
	long	status;

	if (!is_number_str(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ft_free_d(args);
		return (2);
	}
	status = ft_atol(args[1]);
	exitcode = (int)(status % 256);
	if (exitcode < 0)
		exitcode = (exitcode + 256) % 256;
	printf("exit\n");
	ft_free_d(args);
	exit(exitcode);
}

int	ft_exit(char **args, t_shenv **env)
{
	if (env)
		(void)env;
	if (!args || !args[1])
	{
		printf("exit\n");
		ft_free_d(args);
		exit(0);
	}
	if (args[2])
	{
		ft_free_d(args);
		ft_putstr_fd("minishell: exit too many arguments\n", 2);
		return (1);
	}
	return (ft_exit2(args));
}
