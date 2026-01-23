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

// static int	manage_status_in_group(t_cli *cli, const int status)
// {
// 	if (WIFSIGNALED(status))
// 	{
// 		if (status == 2)
// 			write(1, "\n", 1);
// 		cli->last_status = 128 + WTERMSIG(status);
// 		exit(status);
// 	}
// 	if (WIFEXITED(status))
// 		cli->last_status = WEXITSTATUS(status);
// 	else
// 		cli->last_status = 1;
// 	return (cli->last_status);
// }
//
// static t_cli	*search_close_prnts(t_cli *cli)
// {
// 	while (cli)
// 	{
// 		if (cli->op == CL_PRNTS)
// 			return (cli);
// 		cli = cli->next;
// 	}
// 	perror_msh("syntax error", "parenthesis not closed\n");
// 	return (NULL);
// }
//
// static int	open_prnts(t_cli *node, t_cli **cli)
// {
// 	pid_t	pid;
// 	t_cli	*close1;
// 	int		status;
//
// 	close1 = search_close_prnts(node);
// 	if (!close1)
// 		return (2);
// 	apply_redirs(close1);
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("minishell: fork"), 2);
// 	if (pid == 0)
// 		ft_exec(node->next);
// 	set_sig(IGNORE);
// 	waitpid(pid, &status, 0);
// 	set_sig(PARENT);
// 	*cli = close1;
// 	return (manage_status_in_group(node, status));
// }

// int	close_prnts(t_cli *cli, const int status)
// {
//
// 	free_list(cli);
// 	exit(status);
// }

int	exec_ops(t_cli *cli)
{
	int	op;
	int	status;

	if (!cli)
		return (0);
	op = cli->op;
	cli->op = 0;
	status = execute(cli);
	if ((op == AND && !status) || (op == OR && status))
	{
		status = exec_ops(cli->next);
		printf("HOLA\n");
	}
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