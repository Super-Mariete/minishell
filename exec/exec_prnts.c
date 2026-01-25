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

t_cli	*close_prnts_node(t_cli *cli)
{
	int	group;

	group = cli->group;
	while (cli)
	{
		if (cli->group == group && cli->op == CL_PRNTS)
			return (cli);
		cli = cli->next;
	}
	return (cli);
}

static int	handle_aux(t_cli *cli, int *fds, const int mode)
{
	int	status;

	if (mode == 0)
	{
		fds[0] = dup(STDIN_FILENO);
		return (fds[1] = dup(STDOUT_FILENO), 0);
	}
	if (mode == 1)
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
	}
	if (mode == 1 || mode == 2)
		return (close(fds[0]), close(fds[1]), 0);
	if (mode == 4)
	{
		close(fds[0]);
		close(fds[1]);
		exit(2);
	}
	status = cli->status;
	free_list(cli);
	exit(status);
}

pid_t	handle_prnts(t_cli *cli)
{
	pid_t		pid;
	int			status;
	const t_cli	*close_cli;
	int			fds[2];

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
		return (set_sig(CHILD), handle_aux(NULL, fds, 2), pid);
	set_sig(IGNORE);
	waitpid(pid, &status, 0);
	handle_aux(NULL, fds, 1);
	status = manage_status(cli, status);
	cli = close_prnts_node(cli);
	return (cli->status = status, pid);
}

t_cli	*next_node_pipe(t_cli *cli)
{
	int	status;

	if (!cli)
		return (NULL);
	status = cli->status;
	while (cli && cli->next && cli->next->op == PIPE)
		cli = cli->next;
	if (cli)
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
