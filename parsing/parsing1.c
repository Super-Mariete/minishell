/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:00 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:08:43 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	create_file(const t_cli *cli)
{
	int	fd;
	int	mode;

	mode = O_TRUNC;
	if (cli->r_mode == APPEND)
		mode = O_APPEND;
	fd = open(cli->outfile, O_WRONLY | O_CREAT | mode, 0644);
	if (fd < 0)
	{
		perror(cli->outfile);
		return (true);
	}
	close(fd);
	return (false);
}

int	add_args(char *token, t_cli *cli, const int pos)
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
		t = (char **)ft_add_ptr((void **)cli->args, (char *)token, pos);
		if (!t)
			return (perror("malloc"), 0);
		ft_free_d(cli->args);
		cli->args = t;
	}
	cli->n_tokens++;
	return (1);
}

t_cli	*parse_op(const char *token, t_cli **cli)
{
	t_cli	*next_cli;
	int		op;

	op = 0;
	if (!token || !cli || !*cli)
		return (NULL);
	if ((token[0] == '(' || token[0] == ')') && !token[1])
		return (parse_prnts(cli, token[0]));
	if (token[0] == '|' && token[1] == '|')
		op = OR;
	else if (token[0] == '|' )
		op = PIPE;
	else if (token[0] == '&' && token[1] == '&')
		op = AND;
	else
		return (perror_token(token, SYN_ERR), NULL);
	next_cli = init_node((*cli)->n_tokens, (*cli)->env, op);
	if (!next_cli)
		return (perror("minishell: malloc :"), (*cli)->status = 2, NULL);
	next_cli->prev = (*cli);
	(*cli)->next = next_cli;
	(*cli) = (*cli)->next;
	return (next_cli);
}

static char	*cmd_path(char *env_path, const char *cmd, char *t)
{
	int		i;
	char	**path;
	char	*cmd_path;

	if (!env_path)
		return (NULL);
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

int	set_cmd(char *token, t_cli *cli)
{
	if (!token)
		return (0);
	if (!ft_strcmp(token, "echo")
		|| !ft_strcmp(token, "cd") || !ft_strcmp(token, "pwd")
		|| !ft_strcmp(token, "export") || !ft_strcmp(token, "unset")
		|| !ft_strcmp(token, "env") || !ft_strcmp(token, "exit"))
		return (cli->is_builtin = 1, cli->cmd = ft_strdup(token), 1);
	if (ft_strchr(token, '/'))
		cli->cmd = ft_strdup(token);
	else if (token[0])
		cli->cmd = cmd_path(ft_getenv(*cli->env, "PATH"), token, NULL);
	if (!cli->cmd)
		cli->cmd = ft_strdup(token);
	return (1);
}
