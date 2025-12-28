#include "test.h"

sig_atomic_t g_signal;

static void	ft_print_list(t_env *env)
{
	t_env	*node;
	int	i;

	if (!env)
		return ;
	node = env;
	i = 0;
	while (node)
	{
		if (node->key && node->value)
			printf("%s=%s\n", node->key, node->value);
		node = node->next;
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	static t_env	env[VAR_MAX];
	static char		var_arena[ARG_MAX];
	t_msh			msh;

	msh.env_arena = var_arena;
	msh.env = env;
	msh.env_cursor = var_arena;
	// ft_set_sig(PARENT);
	ft_init_var_list(env);
	if (ft_load_env(&msh, envp))
		return (126);
	ft_print_list(env);
	return (0);
}