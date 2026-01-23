/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:48:22 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/21 15:10:10 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cli	*close_prnts_node(const t_cli *cli)
{
	while (cli && cli->op != CL_PRNTS)
		cli = cli->next;
	if (cli)
		return (cli->next);
	return (NULL);
}

pid_t	handle_prnts(t_cli *cli)
{
	pid_t	pid;
	int		status;

	if (cli->op == CL_PRNTS)
	{
		status = cli->last_status;
		free_list(cli);
		exit(status);
	}
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), pid);
	waitpid(pid, &status, 0);
	manage_status(cli, status);
	cli = close_prnts_node(cli);
	cli->last_status = status;
	return (pid);
}

t_cli	*next_node_pipe(t_cli *cli)
{
	while (cli && cli->op == PIPE && cli->next)
		cli = cli->next;
	return (cli);
}

bool	checks_logic(const t_cli *cli)
{
	if (!cli->op || (cli->op == OR && cli->last_status != 0)
			|| (cli->op == AND && cli->last_status == 0))
		return (1);
	return (0);
}
// static void	print_op(const int op)
// {
// 	if (op == PIPE)
// 		printf("PIPE (|)\n");
// 	else if (op == AND)
// 		printf("AND (&&)\n");
// 	else if (op == OR)
// 		printf("OR (||)\n");
// 	else
// 		printf("NONE\n");
// }
//
// static void	print_args(char **args)
// {
// 	int		i;
//
// 	printf("Args: [");
// 	if (args)
// 	{
// 		i = 0;
// 		while (args[i])
// 		{
// 			printf("'%s'", args[i]);
// 			if (args[i + 1])
// 				printf(", ");
// 			i++;
// 		}
// 	}
// 	printf("]\n");
// }
//
// void	print_parser(t_cli *cli)
// {
// 	t_cli	*node;
// 	int		index;
//
// 	if (!cli)
// 		return ;
// 	node = cli;
// 	while (node && node ->prev)
// 		node = node->prev;
// 	index = 0;
// 	while (node)
// 	{
// 		printf("--- Node %d ---\n", index++);
// 		printf("Group: %zu\n", node->group);
// 		printf("Op: ");
// 		print_op(node->op);
// 		printf("Cmd: %s\n", node->cmd ? node->cmd : "(null)");
// 		print_args(node->args);
// 		printf("Infile: %s\n", node->infile ? node->infile : "(null)");
// 		printf("Outfile: %s\n", node->outfile ? node->outfile : "(null)");
// 		printf("Heredoc: %s\n", node->heredoc ? node->heredoc : "(null)");
// 		printf("R_mode: %d\n", node->r_mode);
// 		printf("Is_builtin: %d\n", node->is_builtin);
// 		node = node->next;
// 	}
// 	printf("--- End of List ---\n");
// }