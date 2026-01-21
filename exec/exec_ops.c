/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:48:22 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/21 15:10:10 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_ops(t_cli *cli)
{
	int	op;
	int	status;

	if (!cli)
		return (0);
	op = cli->op;
	cli->op = 0;
	status = execute(cli);
	if (op == AND && !status)
		status = exec_ops(cli->next);
	else if (op == OR && status)
		status = exec_ops(cli->next);
	return (status);
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