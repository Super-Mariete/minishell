/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:28:42 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/23 22:32:52 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_node(t_cli **cli, int *status)
{
	if ((*cli)->op == OP_PRNTS || (*cli)->op == CL_PRNTS)
	{
		if (handle_prnts(*cli))
			*cli = close_prnts_node(*cli);
	}
	else if (!(*cli)->cmd)
	{
		if ((*cli)->heredoc || (*cli)->infile || (*cli)->outfile)
			(*cli)->status = handle_redirs(*cli);
		else
		{
			perror_msh(NULL, "command not found\n");
			(*cli)->status = 127;
		}
	}
	else if ((*cli)->next != NULL && (*cli)->next->op == PIPE)
	{
		*status = execute_pipeline(*cli, -1, -1);
		*cli = next_node_pipe(*cli);
		(*cli)->status = *status;
	}
	else if ((*cli)->is_builtin)
		execute_builtin(*cli);
	else
		execute_command(*cli);
}
