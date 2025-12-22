/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:37 by rafael            #+#    #+#             */
/*   Updated: 2025/12/22 19:43:44 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

sig_atomic_t	g_signal;

int	main(void)
{
	static t_env	env[VAR_MAX];
	static char		var_pool[ARG_MAX];
	
	ft_set_sig(PARENT);
	if (ft_init_var_list(env))
		return (1);
	// free_nodes = NULL;
	// 2.- Get local variables
	// 3.- Get environment variables
	// 4.- Show prompt
}
