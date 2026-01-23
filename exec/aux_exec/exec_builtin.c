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

static char	**exit_args(const t_cli *cli)
{
	char	**ret;
	int		len;
	int		i;

	len = ft_doubleptr_len((void **)cli->args);
	ret = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!ret)
		return (perror("minishell: malloc"), NULL);
	i = 0;
	while (i < len && cli->args[i])
	{
		ret[i] = ft_strdup(cli->args[i]);
		if (!ret[i])
			return (free_tokens(cli->args, i),
				perror("minishell: malloc"), NULL);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

static int	do_exit(t_cli *cli, const int in, const int out,
					int (*builtin)(char **, t_shenv **))
{
	char	**args;

	args = exit_args(cli);
	if (!args[1] || (!args[2] && is_number_str(args[1])))
	{
		close(in);
		close(out);
		free_env(cli->env);
		reset_free(cli);
	}
	return (builtin(args, NULL));
}

int	exec_builtin(t_cli *cli, const int in, const int out)
{
	int		ret;
	int		(*builtin)(char **, t_shenv **);

	if (!cli || !cli->cmd)
		return (1);
	builtin = get_builtin(cli->cmd);
	if (!builtin)
		return (cli->status);
	if (ft_strcmp(cli->cmd, "exit") != 0)
	{
		ret = builtin(cli->args, cli->env);
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
	}
	else
		ret = do_exit(cli, in, out, builtin);
	return (ret);
}
