/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:54 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:38:37 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	print_list(t_cli *cli)
// {
// 	int	i = 0;
// 	int	node = 0;
//
// 	if (!cli)
// 		return ;
// 	while (cli)
// 	{
// 		if (cli->cmd)
// 			printf("cmd %d = %s\n", node, cli->cmd);
// 		if (cli->infile)
// 			printf("infile %d = %s\n", node, cli->infile);
// 		if (cli->outfile)
// 			printf("outfile %d = %s\n", node, cli->outfile);
// 		if (cli->is_builtin)
// 			printf("is_builtin %d = %d\n", node, cli->is_builtin);
// 		if (cli->r_mode)
// 			printf("r_mode %d = %d\n", node, cli->r_mode);
// 		if (cli->heredoc)
// 			printf("heredoc %d = %s\n", node, cli->heredoc);
// 		printf("op %d = %d\n", i, cli->op);
// 		printf("group %d = %zu\n", i, cli->group);
// 		while (cli->args && i < ft_doubleptr_len((void **)cli->args))
// 		{
// 			printf("args[%d] %d = %s\n", i, node, cli->args[i]);
// 			i++;
// 		}
// 		i = 0;
// 		// while (cli->ft_env && cli->ft_env[i])
// 		// {
// 		// 	printf("env[%d] %d = %s\n", i, node, cli->ft_env[i]);
// 		// 	i++;
// 		// }
// 		cli = cli->next;
// 		node++;
// 	}
// }

void	perror_msh(const char *problem, const char *mssg)
{
	write(2, "minishell: ", 11);
	if (problem)
		write(2, problem, ft_strlen(problem));
	write(2, ": ", 2);
	if (mssg)
		write(2, mssg, ft_strlen(mssg));
}

void	perror_token(const char *token, const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (token)
		write(2, token, ft_strlen(token));
	write(2, "\n", 1);
}

void	free_tokens(char **tokens, const size_t n)
{
	int	i;

	i = 0;
	while (tokens && i < n)
	{
		free(tokens[i]);
		i++;
	}
	if (tokens)
		free(tokens);
}

void	free_first_node(t_cli *cli)
{
	free(cli->cmd);
	cli->cmd = nullptr;
	free(cli->heredoc);
	cli->heredoc = nullptr;
	cli->heredoc_fd = -1;
	free(cli->infile);
	cli->infile = nullptr;
	free(cli->outfile);
	cli->outfile = nullptr;
	ft_free_d(cli->args);
	cli->args = nullptr;
	cli->is_builtin = 0;
	cli->r_mode = 0;
	cli->group = 0;
	cli->op = 0;
}

t_cli	*init_node(const size_t len, t_shenv **env, const int op)
{
	t_cli	*cli;

	if (len == 0)
		return (nullptr);
	cli = (t_cli *)ft_calloc(1, sizeof(t_cli));
	if (!cli)
		return (perror("malloc : "), nullptr);
	cli->cmd = nullptr;
	cli->args = nullptr;
	cli->ft_env = env;
	if (env && !cli->ft_env)
		perror("malloc : ");
	cli->infile = nullptr;
	cli->outfile = nullptr;
	cli->heredoc = nullptr;
	cli->heredoc_fd = -1;
	cli->is_builtin = 0;
	cli->next = nullptr;
	cli->r_mode = WRITE;
	cli->n_tokens = len;
	cli->group = 1;
	cli->op = op;
	cli->status = 0;
	cli->last_status = 0;
	return (cli);
}
