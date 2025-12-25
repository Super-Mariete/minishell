/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:00 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:16:56 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int     ft_args(char *token, t_cli *cli, int pos)
{
	char	**t;

	if (!token || !cli)
		return (0);
	if (!cli->args)
	{
		cli->args = (char **)ft_calloc(2, sizeof(char *));
		if (!cli->args)
			return (perror("malloc"), 0);
		cli->args[1] = NULL;
		cli->args[0] = ft_strdup(token);
		if (!cli->args[0])
			return (perror("malloc"), 0);
	}
	else
	{
		t = (char **)ft_add_ptr((void *)cli->args, (char *)token, pos);
		if (!t)
			return (perror("malloc"), 0);
		ft_free_d(cli->args);
		cli->args = t;
	}
	cli->n_tokens++;
	return (1);
}

t_cli	*ft_parse_op(char *token, t_cli *cli)
{
	char	*pipe;
	t_cli	*next_cli;
	int		op;

	op = 0;
	if (!token || !cli)
		return (perror("parse op !token || !cli"), NULL);
	if (token[0] == '|' && token[1] == '|')
		op = OR;
	else if (token[0] == '|' )
		op = PIPE;
	else if (token[0] == '&' && token[0] == '&')
		op = AND;
	else
		return (perror("invalid token parse_op"), NULL);
	cli->op = op;
	next_cli = ft_init_node(cli->n_tokens, cli->env, 0);
	if (!next_cli)
		return (perror("malloc : "), cli->status = 2, NULL);
	return (next_cli);
}

char	*ft_cmd_path(char *env_path, char *cmd)
{
	int		i;
	char	**path;
	char	*cmd_path;
	char	*t;

	i = 0;
	path = ft_split(env_path, ':');
	if (!path)
		return (perror("malloc"), NULL);
	while (path[i])
	{
		t = ft_strjoin(path[i], "/");
		if (!t)
			return (ft_free_d(path), perror("malloc"), NULL);
		cmd_path = ft_strjoin(t, cmd);
		free(t);
		if (!cmd_path)
		    return (ft_free_d(path), perror("malloc"), NULL);
		if (!access(cmd_path, X_OK))
			return (ft_free_d(path), cmd_path);
		free(cmd_path);
		i++;
	}
	return (ft_free_d(path), NULL);
}


int	ft_cmd(char	*token, t_cli *cli)
{
	if (!token)
		return (0);
	if (!ft_strcmp(token, "echo")
	|| !ft_strcmp(token, "cd") || !ft_strcmp(token, "pwd")
	|| !ft_strcmp(token, "export") || !ft_strcmp(token, "unset")
	|| !ft_strcmp(token, "env") || !ft_strcmp(token, "exit"))
		return (cli->is_builtin = 1, cli->cmd = ft_strdup(token), 1);
	if (token && ft_strchr(token, '/'))
		cli->cmd = ft_strdup(token);
	else
		cli->cmd = ft_cmd_path(getenv("PATH"), token);
	if (!cli->cmd)
		cli->cmd = ft_strdup(token);
	return (1);
}