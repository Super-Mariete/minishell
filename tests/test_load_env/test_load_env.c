#include "../../includes/minishell.h"

sig_atomic_t g_signal;

// static char	*ft_next_gap(const char *buffer)
// {
// 	size_t	i;
// 	const char *ret;

// 	i = 0;
// 	ret = buffer;
// 	while (i < ARG_MAX)
// 	{
// 		if (buffer[i] == 0 && i + 1 < ARG_MAX && buffer[i + 1] == 0)
// 			return ((char *)ret + i + 1);
// 		i++;
// 	}
// 	return (NULL);
// }

static void	ft_print_list(t_env *env)
{
	size_t	i;
	size_t	len;
	char	*buffer;

	if (!env)
		return ;
	buffer = env->arena;
	i = 0;
	while (i < ARG_MAX)
	{
		if (buffer[0] == 0 && (i + 1 >= ARG_MAX || buffer[1] == 0))
		{
			printf("END OF ARENA\n");
			fflush(stdout);
			return ;
		}
		// if (*buffer == 2)
		// 	printf("is_exported = %d | ", 2);
		// else
		// 	printf("is_exported = %d | ", 1);
		fflush(stdout);
		buffer++;
		i++;
		printf("%s\n", buffer);
		fflush(stdout);
		len = (ft_strlen(buffer) + 1);
		buffer += len;
		i += len;
	}
}

int	main(int argc, char **argv, char **envp)
{
	static char	var_arena[ARG_MAX];
	t_env		env;
	t_msh		msh;

	(void)argc;
	(void)argv;
	msh.env = &env;
	msh.env->arena = var_arena;
	msh.env->last = NULL;
	msh.env->cursor = var_arena;
	msh.env->head = NULL;
	ft_set_sig(PARENT);
	if (ft_load_env(msh.env, envp))
		return (126);
	ft_print_list(msh.env);
}