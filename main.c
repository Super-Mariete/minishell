/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:32:37 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 10:07:23 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

sig_atomic_t	g_signal;

static void	ft_init_structs(t_msh *msh)
{
	static char		var_arena[ARG_MAX];
	static t_env	env;
	static t_term	term;
	static int		status;

	msh->env = &env;
	msh->env->arena = var_arena;
	msh->env->last = NULL;
	msh->env->cursor = var_arena;
	msh->env->head = NULL;
	msh->term = &term;
	msh->status = &status;
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	msh;

	(void)argc;
	(void)argv;
	ft_init_structs(&msh);
	if (isatty(STDIN_FILENO))
	{
		ft_set_sig(PARENT);
		if (ft_init_term(msh.term))
			return (errno);
		if (tcsetattr(STDIN_FILENO, TCSANOW, &(msh.term->raw_mode)) == -1)
			return (errno);
	}
	if (ft_load_env(msh.env, envp))
		return (126);
	ft_readline(&msh);
	if (isatty(STDIN_FILENO))
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, &(msh.term->canon_mode)) == -1)
			return (errno);
	}
	return (*(msh.status));
}
