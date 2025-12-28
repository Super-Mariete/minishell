/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:37 by rafael            #+#    #+#             */
/*   Updated: 2025/12/28 01:43:11 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	static t_env	env[VAR_MAX];
	static char		var_arena[ARG_MAX];
	t_msh			msh;

	(void)argc;
	(void)argv;
	msh.env_arena = var_arena;
	msh.env = env;
	msh.env_cursor = var_arena;
	ft_set_sig(PARENT);
	ft_init_var_list(env);
	if (ft_load_env(&msh, envp))
		return (126);
	// 2.- Get variables
	// 3.- Get environment variables
	// 4.- Show prompt
}
