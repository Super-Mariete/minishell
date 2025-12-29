#include "../includes/minishell.h"

static void	ft_free_node(t_env *node)
{
	ft_bzero((void *)node->key, ft_strlen(node->key));
	ft_bzero((void *)node->value, ft_strlen(node->value));
	node->key = NULL;
	node->value = NULL;
	node->is_exported = 0;
}

static void	ft_add_empty_node(t_env *env, t_env *node)
{
	t_env	*temp;
	int	i;

	i = 0;
	temp = env;
	while (i < VAR_MAX && temp)
	{
		if (!temp->next)
			break ;
		if (!temp->key && node != temp)
		{
			node->next = temp->next;
			temp->next = node;
			return ;
		}
		temp = temp->next;
		i++;
	}
	if (temp)
		temp->next = node;
	node->next = NULL;
	return ;
}

static	void	ft_unset_node(t_msh *msh)
{
	t_env	*node;
	t_env	*next;

	node = msh->head;
	ft_free_node(node);
	if (node->next)
	{
		msh->head = node->next;
		ft_add_empty_node(msh->head, node);
	}
	return ;
}

static	void	ft_unset_node(t_env *node, t_env *next)
{
	ft_free_node(next);
	if (next->next)
	{
		node->next = next->next;
		ft_add_empty_node(node->next, next);
	}
	return ;
}

void ft_unset(t_msh *msh, char *key)
{
	t_env	*node;
	t_env	*next;

	if (!key)
		return ;
	node = msh->head;
	if (node->key && !ft_strcmp(node->key, key))
	{
		ft_unset_node(msh);
		return ;
	}
	next = node->next;
	while (node && next)
	{
		if (next->key && !ft_strcmp(next->key, key))
		{
			ft_free_node(next);
			if (next->next)
			{
				node->next = next->next;
				ft_add_empty_node(node->next, next);
			}
			return ;
		}
		node = node->next;
		if (node)
			next = node->next;
	}
	return ;
}