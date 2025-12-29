#include "../includes/minishell.h"

static void	ft_free_node(t_env *node)
{
	ft_bzero((void *)node->key, ft_strlen(node->key));
	ft_bzero((void *)node->value, ft_strlen(node->value));
	node->key = NULL;
	node->value = NULL;
	node->is_exported = 0;
}

// static	int	ft_is_gap(const char *buffer)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	while (i < ARG_MAX)
// 	{
// 		if (buffer[i] == 0 && (++i) < ARG_MAX && buffer[i] == 0)
// 		{
// 			if (i > 0 && buffer[i - 1] != 0)
// 				return (0);
// 			j = 0;
// 			while ((i + j) <= ARG_MAX)
// 			{
// 				if ((i + j) == ARG_MAX)
// 					return (-1);
// 				if (buffer[i + j] != 0)
// 					return (j);
// 				j++;
// 			}
// 		}
// 		i++;
// 	}
// 	return (0);
// }

static void	ft_add_empty_node(t_env *env, t_env *node)
{
	t_env	*temp;
	int	i;

	i = 0;
	temp = env;
	while (i < VAR_MAX && temp)
	{
		if (!temp->key && node != temp)
		{
			node->next = temp->next;
			temp->next = node;
			return ;
		}
		if (!temp->next)
			break ;
		temp = temp->next;
		i++;
	}
	temp->next = node;
	node->next = NULL;
	return ;
}

void ft_unset(t_msh *msh, char *key)
{
	t_env	*node;
	t_env	*next;

	if (!key)
		return ;
	node = msh->env;
	if (node->key && !ft_strcmp(node->key, key))
	{
		ft_free_node(node);
		ft_add_empty_node(node->next, node);
		return ;
	}
	next = node->next;
	while (node && next)
	{
		if (next->key && !ft_strcmp(next->key, key))
		{
			ft_free_node(next);
			node->next = next->next;
			ft_add_empty_node(next->next, next);
			return ;
		}
		node = node->next;
		if (node)
			next = node->next;
	}
	return ;
}