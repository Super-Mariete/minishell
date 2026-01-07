/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 10:09:44 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 18:10:58 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_print_ast(t_ast *ast)
{
	t_node	*node;
	int		i;
	size_t	len;

	node = ast->first;
	while (node && node->left)
		node = node->left;
	while (node)
	{
		i = 0;
		len = 0;
		printf("\nnode = %p\n", (void *)node);
		if (node->cmd)
		{
			printf("append = %d\n", node->cmd->append);
			printf("cmd = %s\n", node->cmd->cmd);
			printf("infile = %s\n", node->cmd->infile);
			printf("outfile = %s\n", node->cmd->outfile);
			printf("heredoc = %s\n", node->cmd->heredoc);
			printf("n_args = %d\n", node->cmd->n_args);
			printf("is_bultin = %d\n", node->cmd->is_builtin);
			while (i < node->cmd->n_args)
			{
				printf("arg[%d] = %s\n", i, (node->cmd->args + len));
				len += ft_strlen(node->cmd->args + len) + 1;
				i++;
			}
		}
		printf("type = %d\n", node->type);
		printf("prev = %p\n", (void *)node->prev);
		if (node->prev)
			printf("prev->type = %d\n", node->prev->type);
		printf("left = %p\n", (void *)node->left);
		printf("right = %p\n", (void *)node->right);
		if (!node->prev)
			return ;
		if (node->prev->right == node)
		{
			if (!node->prev->prev)
				return ;
			node = node->prev->prev->right;
		}
		else
			node = node->prev->right;
	}
	return ;
}

t_node	*ft_next_node(t_ast *ast, int *status)
{
	t_node		*next;
	t_node		*node;

	node = ast->current;
	if (ast->n_nodes >= CMD_MAX)
	{
		write(2, "minishell: CMD_MAX exceeded\n", 28);
		*status = E2BIG;
		return (NULL);
	}
	next = &(node[ast->n_nodes + 1]);
	node->left = next;
	next->prev = node;
	return (node);
}
