/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-ped <made-ped@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:06:56 by made-ped          #+#    #+#             */
/*   Updated: 2025/10/31 19:46:55 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** Función callback para imprimir una variable de entorno.
** Se adapta a ft_lstiter (recibe un void *content).
*/
static void print_env_var(void *content)
{
    t_shenv *node;

    node = (t_shenv *)content;
    if (!node || !node->var)
        return;
    if (ft_strchr(node->var, '='))
        printf("%s\n", node->var);
}

/*
** Implementación de ft_env: imprime las variables del entorno.
*/
int ft_env(char **args, t_shenv **env)
{
    (void)args;
    if (!env || !*env)
        return (0);
    ft_lstiter((t_list *)(*env), print_env_var);
    return (0);
}
