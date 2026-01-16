/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:49:44 by made-ped          #+#    #+#             */
/*   Updated: 2025/12/18 19:33:39 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exec_builtin(t_cli *cli)
{
	int	(*builtin)(char **, t_shenv **);

	if (!cli || !cli->cmd)
		return (1);
	builtin = get_builtin(cli->cmd);
	if (!builtin)
		return (cli->last_status);
	return (builtin(cli->args, cli->ft_env));
}
