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
		printf("node[%d] = %p\n", i, (void *)node);
		printf("node->next = %p\n", (void *)node->next);
		printf("node->key = %s\n", node->key);
		printf("node->value = %s\n", node->value);
		printf("node->is_exported = %d\n\n", node->is_exported);
		node = node->next;
		i++;
	}
}

int	main()
{
	static t_env	env[ARG_MAX];

	if (ft_init_var_pool(env))
		return (1);
	ft_print_list(env);
}