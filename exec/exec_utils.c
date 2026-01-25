/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:24:37 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/16 13:52:51 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirs(const t_cli *cli)
{
	int	stdin_save;
	int	stdout_save;

	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if (apply_redirs(cli))
	{
		dup2(stdin_save, STDIN_FILENO);
		dup2(stdout_save, STDOUT_FILENO);
		close(stdin_save);
		close(stdout_save);
		return (1);
	}
	dup2(stdin_save, STDIN_FILENO);
	dup2(stdout_save, STDOUT_FILENO);
	close(stdin_save);
	close(stdout_save);
	return (0);
}

int	execute_builtin(t_cli *cli)
{
	int	stdin_save;
	int	stdout_save;
	int	status;

	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if (apply_redirs(cli))
	{
		close(stdin_save);
		close(stdout_save);
		return (1);
	}
	status = exec_builtin(cli, stdin_save, stdout_save);
	close(stdin_save);
	close(stdout_save);
	cli->status = status;
	return (status);
}

int	exec_builtin_child(const t_cli *cli)
{
	int	ret;
	int	(*builtin)(char **, t_shenv **);

	builtin = get_builtin(cli->cmd);
	if (!builtin)
		return (1);
	ret = builtin(cli->args, cli->env);
	return (ret);
}

/* Busca si el comando es un builtin y devuelve su función si existe */
int	(*get_builtin(char *cmd))(char **args, t_shenv **env)
{
	int						i;
	static const t_builtin	builtins[] = {
	{"echo", ft_echo},
	{"pwd", ft_pwd},
	{"cd", ft_cd},
	{"export", ft_export},
	{"unset", ft_unset},
	{"env", ft_env},
	{"exit", ft_exit},
	{NULL, NULL}
	};

	i = 0;
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
			return (builtins[i].func);
		i++;
	}
	return (NULL);
}
