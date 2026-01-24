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
	write(2, "'\n", 2);
}

void	free_tokens(char **tokens, const size_t n)
{
	size_t	i;

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
	if (cli->heredoc_fd != -1)
		close(cli->heredoc_fd);
	cli->heredoc_fd = -1;
	free(cli->infile);
	cli->infile = nullptr;
	free(cli->outfile);
	cli->outfile = nullptr;
	ft_free_d(cli->args);
	cli->args = nullptr;
	cli->is_builtin = 0;
	cli->r_mode = 0;
	cli->op = 0;
	cli->next = NULL;
}

t_cli	*init_node(const size_t len, t_shenv **env, const int op)
{
	t_cli	*cli;

	if (len == 0)
		return (nullptr);
	cli = (t_cli *)ft_calloc(1, sizeof(t_cli));
	if (!cli)
		return (perror("minishell: malloc"), nullptr);
	cli->cmd = nullptr;
	cli->args = nullptr;
	cli->env = env;
	if (env && !cli->env)
		perror("minishell: malloc");
	cli->infile = nullptr;
	cli->outfile = nullptr;
	cli->heredoc = nullptr;
	cli->heredoc_fd = -1;
	cli->is_builtin = 0;
	cli->next = nullptr;
	cli->r_mode = WRITE;
	cli->n_tokens = len;
	cli->op = op;
	cli->group = 0;
	if (cli->prev)
		cli->group = cli->prev->group;
	return (cli->status = 0, cli->prev = NULL, cli);
}
