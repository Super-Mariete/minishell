#include "includes/minishell.h"

sig_atomic_t g_signal;

int    main()
{
	static t_env	env[ARG_MAX];
	// t_env			*free_nodes;

 	ft_set_sig(PARENT);
	if (ft_init_var_pool(env))
		return (1);
	// free_nodes = NULL;
    // 2.- Get local variables
    // 3.- Get environment variables
    // 4.- Show prompt
}
