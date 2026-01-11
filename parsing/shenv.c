#include "../minishell.h"

void	free_env(t_shenv **ft_env)
{
	t_shenv *node;
	t_shenv *next;

	if (!ft_env || !*ft_env)
		return ;
	node = *ft_env;
	while (node)
	{
		next = node ->next;
		free(node->var);
		free(node);
		node = next;
	}
	*ft_env = NULL;
}

int     shenv_len(t_shenv *ft_env)
{
        int     len;

        len = 0;
        if (!ft_env)
                return (0);
        while (ft_env)
        {
                len++;
                ft_env = ft_env->next;
        }
        return (len);
}

char    **getshenv(t_shenv *ft_env)
{
        t_shenv         *node;
        char    **r;
        int             len;
        int             i;

        if (!ft_env)
                return (NULL);
        len = shenv_len(ft_env);
        r = (char **)ft_calloc(len + 1, sizeof(char *));
        if (!r)
                return (perror("minishell : malloc : "), NULL);
        i = 0;
        node = ft_env;
        while (node)
        {
                r[i] = ft_strdup(node->var);
                if (node->var && !r[i])
                        return (perror("minishell : malloc : "), NULL);
                i++;
                node = node->next;
        }
        r[i] = NULL;
        return (r);
}

t_shenv *load_env(char **envp)
{
        int             i;
        int             len;
        t_shenv *ft_env;
        t_shenv *node;

        if (!envp)
                return (NULL);
        ft_env = (t_shenv *)ft_calloc(1, sizeof(t_shenv));
        if (!ft_env)
                return (perror("minishell : malloc : "), NULL);
        i = 0;
        node = ft_env;
        while (envp[i])
        {
                node->var = ft_strdup(envp[i]);
                if (!node->var && envp[i] && envp[i][0])
                        return (free_env(&ft_env), perror("minishell : malloc : "), NULL);
                if (!envp[++i])
                        break ;
                node->next = (t_shenv *)ft_calloc(1, sizeof(t_shenv));
                if (!node->next)
                        return (free_env(&ft_env), perror("minishell : malloc : "), NULL);
                node = node->next;
        }
        return (node->next = NULL, ft_env);
}