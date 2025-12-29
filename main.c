/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:37 by rafael            #+#    #+#             */
/*   Updated: 2025/12/29 20:56:23 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	static char	var_arena[ARG_MAX];
	t_msh		msh;
	t_env		env;
	t_term		term;
	int			status;

	(void)argc;
	(void)argv;
	msh.env = &env;
	msh.env->arena = var_arena;
	msh.env->last = NULL;
	msh.env->cursor = var_arena;
	msh.env->head = NULL;
	msh.term = &term;
	ft_set_sig(PARENT);
	if (ft_load_env(msh.env, envp))
		return (126);
	if (ft_init_term(&term))
		return (errno);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(term.raw_mode)) == -1)
		return (errno);
	status = ft_readline(&msh);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(term.canon_mode)) == -1)
		return (errno);
	return (status);
}
