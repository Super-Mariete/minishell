#include "../includes/minishell.h"

int	ft_init_var_pool(t_env *pool)
{
	t_env	*node;
	t_env	*temp;
	int	i;

	if (!pool || ARG_MAX <= 0)
		return (1);
	i = 0;
	node = pool;
	temp = pool;
	temp++;
	while (i < ARG_MAX - 1)
	{
		node->next = temp;
		i++;
		temp++;
		node = node->next;
	}
	node->next = NULL;
	return (0);
}