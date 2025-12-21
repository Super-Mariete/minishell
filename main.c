#include "includes/minishell.h"

static void	ft_print_list(t_env *env)
{
	t_env	*node;
	int	i;

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

static void	ft_init_var_pool(t_env *pool)
{
	t_env	*node;
	t_env	*temp;
	int	i;

	i = 0;
	node = pool;
	temp = pool;
	temp++;
	printf("temp = %p\n", (void *)temp);
	printf("ARG_MAX = %d\n", ARG_MAX);
	while (i < ARG_MAX - 1)
	{
		node->next = temp;
		i++;
		temp++;
		node = node->next;
	}
	node->next = NULL;
}

int    main()
{
	static t_env	env[ARG_MAX];
	// t_env			*free_nodes;

 	ft_set_sig(PARENT);
	ft_init_var_pool(env);
	ft_print_list(env);
	// free_nodes = NULL;
    // 2.- Get local variables
    // 3.- Get environment variables
    // 4.- Show prompt
}
