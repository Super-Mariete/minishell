/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:37 by rafael            #+#    #+#             */
/*   Updated: 2025/12/29 19:03:07 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	static char	var_arena[ARG_MAX];
	t_msh		msh;
	t_env		env;

	(void)argc;
	(void)argv;
	msh.env = &env;
	msh.env->arena = var_arena;
	msh.env->last = NULL;
	msh.env->cursor = var_arena;
	msh.env->head = NULL;
	ft_set_sig(PARENT);
	ft_init_var_list(msh.env);
	if (ft_load_env(&msh, envp))
		return (126);
	// 1.- Show prompt
}
