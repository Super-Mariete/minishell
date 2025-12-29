#include "test.h"

sig_atomic_t	g_signal;

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
		{
			printf("%p=%p\n", node->key, node->value);
			printf("%s=%s\n", node->key, node->value);
			fflush(stdout);
		}
		if (node->key)
			i++;
		node = node->next;
	}
}

static char	*ft_next_gap(const char *buffer)
{
	size_t	i;
	const char *ret;

	i = 0;
	ret = buffer;
	while (i < ARG_MAX)
	{
		if (i == ARG_MAX)
			return ((char *)ret + i);
		if (buffer[i] == 0 && i + 1 < ARG_MAX && buffer[i + 1] == 0)
			return ((char *)ret + i + 1);
		i++;
	}
	return (0);
}

static void	ft_add_empty_node(t_env *env, t_env *node)
{
	t_env	*temp;
	int	i;

	if (!env)
		return ;
	i = 0;
	temp = env;
	while (i < VAR_MAX && temp)
	{
		if (temp)
			temp->next = node;
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
	node->next = NULL;
	return ;
}

static void ft_unset_vars(t_msh *msh)
{
	int	i;
	t_env	*node;
	t_env	*next;

	node = msh->env;
	i = 0;
	next = node->next;
	while (node && next)
	{
		if (i > 5 || (i % 2 == 0 && next->key))
		{
			ft_bzero((void *)next->key, ft_strlen(next->key));
			ft_bzero((void *)next->value, ft_strlen(next->value));
			next->key = NULL;
			next->value = NULL;
			node->next = next->next;
			if (!next->next)
				return ;
			ft_add_empty_node(next->next, next);
			i++;
			continue ;
		}
		i++;
		node = node->next;
		if (node)
			next = node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	static t_env	env[VAR_MAX];
	static char		var_arena[ARG_MAX];
	t_msh			msh;
	(void)argc;
	(void)argv;
	msh.env_arena = var_arena;
	msh.env = env;
	msh.env_cursor = var_arena;
	ft_init_var_list(env);
	ft_load_env(&msh, envp);
	ft_unset_vars(&msh);
	ft_print_list(msh.env);
	printf("\n");
	ft_refill_var_buffer(var_arena, msh.env);
	char	*buffer = ft_next_gap(var_arena);
	ft_print_list(msh.env);
	printf("\nnext_gap at + %lu", (size_t)buffer - (size_t)var_arena);
}