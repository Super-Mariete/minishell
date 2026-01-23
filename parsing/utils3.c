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
	if (token == '(')
	{
		(*cli)->op = OP_PRNTS;
		(*cli)->next = init_node((*cli)->n_tokens, (*cli)->env, 0);
		if (!(*cli)->next)
		{
			reset_list(*cli);
			return (perror("minishell: malloc :"), NULL);
		}
		(*cli)->next->prev = (*cli);
		*cli = (*cli)->next;
		return (*cli);
	}
	(*cli)->next = init_node((*cli)->n_tokens, (*cli)->env, CL_PRNTS);
	if (!(*cli)->next)
	{
		reset_list(*cli);
		return (perror("minishell: malloc :"), NULL);
	}
	(*cli)->next->prev = (*cli);
	*cli = (*cli)->next;
	return (*cli);
}
