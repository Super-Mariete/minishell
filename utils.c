#include "includes/minishell.h"
/* @brief Returns the next position of a gap of free memory in a buffer, 
or NULL if there are no gaps
 */
static char	*ft_next_gap(const char *buffer)
{
	size_t	i;
	const char *ret;

	i = 0;
	ret = buffer;
	while (i < ARG_MAX)
	{
		if (buffer[i] == 0 && i + 1 < ARG_MAX && buffer[i + 1] == 0)
			return ((char *)ret + i + 1);
		i++;
	}
	return (NULL);
}

static t_env	*ft_reset_ptrs(t_env *env, char *old_ptr, char *new_ptr)
{
	t_env	*node;
	
	node = env;
	while (node)
	{
		if (!ft_strcmp(node->key, old_ptr))
		{
			node->key = new_ptr;
			return (node);
		}
		if (!ft_strcmp(node->value, old_ptr))
		{
			node->value = new_ptr;
			return (node);
		}
		node = node->next;
	}
	return (NULL);
}

void	ft_refill_var_buffer(char *buffer, t_env *env)
{
	size_t	string_len;
	char	*next_string;
	char	*temp;
	t_env	*node;

	temp = buffer;
	node = env;
	while ((size_t)temp - (size_t)buffer < ARG_MAX)
	{
		temp = ft_next_gap(temp);
		if (!temp)
			return ;
		next_string = temp;
		while (*next_string == 0)
		{
			next_string++;
			if (next_string - buffer >= ARG_MAX)
				return ;
		}
		node = ft_reset_ptrs(node, next_string, temp);
		string_len = ft_strlen(next_string);
		ft_strncpy(temp, next_string, string_len);
		ft_bzero(next_string, string_len);
		temp += string_len;
	}
	return ;
}

size_t	ft_buffercpy(const char *src, char *dest, size_t size)
{
	ft_strncpy(dest, src, size);
	if (*(dest + size) != 0)
		return (1);
	return (0);
}