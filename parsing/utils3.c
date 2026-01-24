/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:02:30 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/23 21:32:08 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cli	*parse_prnts(t_cli **cli, const char token)
{
	static int	group;

	if (token == '(')
	{
		group++;
		(*cli)->op = OP_PRNTS;
		(*cli)->next = init_node((*cli)->n_tokens, (*cli)->env, 0);
		if (!(*cli)->next)
			return (reset_list(*cli), perror("minishell: malloc"), NULL);
		(*cli)->next->prev = (*cli);
		*cli = (*cli)->next;
		(*cli)->group = group;
		return (*cli);
	}
	group--;
	if (group < 0)
		return (perror_token(")", SYN_ERR), NULL);
	(*cli)->next = init_node((*cli)->n_tokens, (*cli)->env, CL_PRNTS);
	if (!(*cli)->next)
		return (reset_list(*cli), perror("minishell: malloc"), NULL);
	(*cli)->next->prev = (*cli);
	*cli = (*cli)->next;
	(*cli)->group = group;
	return (*cli);
}
