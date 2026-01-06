/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 10:09:44 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 10:10:29 by rafael           ###   ########.fr       */
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
		printf("node = %p\n", (void *)node);
		printf("append = %d\n", node->append);
		printf("type = %d\n", node->type);
		printf("prev = %p\n", (void *)node->prev);
		printf("left = %p\n", (void *)node->left);
		printf("right = %p\n", (void *)node->right);
		printf("cmd = %s\n", node->cmd);
		printf("infile = %s\n", node->infile);
		printf("outfile = %s\n", node->outfile);
		printf("heredoc = %s\n", node->heredoc);
		printf("n_args = %d\n", node->type);
		printf("is_bultin = %d\n", node->is_builtin);
		while (i < node->n_args)
		{
			printf("arg[%d] = %s\n", i, (node->args + len));
			len += ft_strlen(node->args + 1);
			i++;
		}
		if (!node->prev && node->right == node)
			return ;
		if (!node->prev)
			node = node->right;
		else if (node->prev->right)
			node = node->prev->right;
		else
			node = node->prev;
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
