/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:03:35 by made-ped          #+#    #+#             */
/*   Updated: 2025/12/18 12:26:59 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// Busca si el comando es un builtin y devuelve su función si existe
int (*get_builtin(char *cmd))(char **, t_shenv **)
{
	static const t_builtin builtins[] = {
		{"echo", ft_echo},
		{"pwd", ft_pwd},
		{"cd", ft_cd},
		{"export", ft_export},
		{"unset", ft_unset},
		{"env", ft_env},
		{"exit", ft_exit},
		{NULL, NULL}
	};

	int i = 0;
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
			return (builtins[i].func);
		i++;
	}
	return (NULL);
}

int	ft_execute(t_cli *cli)
{
	int	stdin_save;
	int	stdout_save;
	int	status;
//	int (*builtin)(char **, t_shenv **); // ← ahora tiene punto y coma y tipo correcto

	if (!cli || !cli->cmd)
		return (0);

/*	builtin = get_builtin(cli->cmd);
	if (builtin)
		return (builtin(cli->args, cli->env));*/
	if (get_builtin(cli->cmd) && !has_pipe(cli))
	{
		stdin_save = dup(STDIN_FILENO);
		stdout_save = dup(STDOUT_FILENO);

		if (apply_redirs(cli))
			return(1);
		status = exec_builtin(cli);
		dup2(stdin_save, STDIN_FILENO);
		dup2(stdout_save, STDOUT_FILENO);
		close (stdin_save);
		close(stdout_save);
		cli->last_status = status;
		return(status);
	}
	//TODO enteros y pipes;
	return (0);
}

