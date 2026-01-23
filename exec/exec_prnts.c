/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_prnts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:48:22 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/23 12:00:00 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cli	*close_prnts_node(const t_cli *cli)
{
	while (cli && cli->next && cli->next->op != CL_PRNTS)
		cli = cli->next;
	return (cli->next);
}

static void	handle_aux(t_cli *cli, int *fds, const int mode)
{
	if (mode == 0)
	{
		fds[0] = dup(STDIN_FILENO);
		fds[1] = dup(STDOUT_FILENO);
		return ;
	}
	if (mode == 1)
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
	}
	if (mode == 1 || mode == 2)
	{
		close(fds[0]);
		close(fds[1]);
		return ;
	}
	if (mode == 4)
	{
		free_list(cli);
		close(fds[0]);
		close(fds[1]);
		exit(2);
	}
	exit(cli->status);
}

pid_t	handle_prnts(t_cli *cli)
{
	pid_t	pid;
	int		status;
	t_cli	*close_cli;
	int		fds[2];

	if (cli->op == CL_PRNTS)
		handle_aux(cli, NULL, 3);
	close_cli = close_prnts_node(cli);
	handle_aux(NULL, fds, 0);
	if (apply_redirs(close_cli))
		handle_aux(cli, fds, 4);
	pid = fork();
	if (pid < 0)
	{
		handle_aux(NULL, fds, 1);
		return (perror("minishell: fork"), pid);
	}
	if (pid == 0)
		return (handle_aux(NULL, fds, 2), pid);
	waitpid(pid, &status, 0);
	handle_aux(NULL, fds, 1);
	manage_status(cli, status);
	cli = close_prnts_node(cli);
	cli->status = status;
	return (pid);
}

t_cli	*next_node_pipe(t_cli *cli)
{
	int	status;

	status = cli->status;
	while (cli && cli->next && cli->next->op == PIPE)
		cli = cli->next;
	cli->status = status;
	return (cli);
}

bool	checks_logic(const t_cli *cli)
{
	if (cli->op == OP_PRNTS || cli->op == CL_PRNTS)
		return (true);
	if (!cli->op || (cli->op == OR && cli->status != 0)
		|| (cli->op == AND && cli->status == 0))
		return (true);
	return (false);
}
