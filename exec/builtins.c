#include "../includes/minishell.h"

static t_env	*ft_add_empty_var(t_env	*env, t_env *node)
{
	t_env	*temp;
	t_env	*prev;
	int	i;

	i = 0;
	prev = NULL;
	temp = env;
	while (i < VAR_MAX && node)
	{
		if (!temp->key && prev)
		{
			prev->next = node;
			node->next = temp;
			return (env);
		}
		if (!temp->key && !prev)
		{
			temp->next = node;
			return (temp);
		}
		prev = temp;
		temp = temp->next;
		i++;
	}
	temp->next = node;
	node->next = NULL;
	return (env);
}

void ft_unset(t_msh *msh, char *key)
{
	int	i;
	t_env	*node;
	t_env	*prev_node;

	node = msh->env;
	i = 0;
	prev_node = NULL;
	while (i < VAR_MAX && node)
	{
		if (ft_strcmp(key, node->key))
		{
			ft_bzero((void *)node->key, ft_strlen(node->key));
			ft_bzero((void *)node->value, ft_strlen(node->value));
			node->is_exported = 0;
			if (prev_node)
			{
				prev_node->next = node->next;
			}
			msh->env = ft_add_empty_var(node->next, node);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
}