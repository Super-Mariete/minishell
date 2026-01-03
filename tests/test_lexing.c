#include "test.h"

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
	ft_set_sig(PARENT);
	if (ft_load_env(msh.env, envp))
		return (126);
	if (ft_init_term(msh.term))
		return (errno);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(msh.term->raw_mode)) == -1)
		return (errno);
	*(msh.status) = ft_readline(&msh);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(msh.term->canon_mode)) == -1)
		return (errno);
	return (*(msh.status));
}