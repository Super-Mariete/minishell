/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:21:10 by rafael-m          #+#    #+#             */
/*   Updated: 2025/10/31 19:32:14 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 * Itera la lista ’lst’ y aplica la función ’f’ en el contenido de cada nodo. 
 * lst: un puntero al primer nodo.
 * f: un puntero a la función que utilizará cada nodo.*/

/*
static void     to_uppercase_i(void *s)
{
	unsigned char	*d;

	d = (unsigned char	*)s;
        while (*d)
	{
		if (*d >= 'a' && *d <= 'z')
                	*d = *d - 32;
		d++;
	}
}
*/

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
int	main(void)
{
	int	lg;
	t_list *node1 = ft_lstnew(strdup("Hola"));
	t_list *node2 = ft_lstnew(strdup("que"));
	t_list *node3 = ft_lstnew(strdup("tal"));
	t_list *node4 = ft_lstnew(strdup("Adios"));

	ft_lstadd_back(&node1, node2);
	ft_lstadd_back(&node1, node3);
	ft_lstadd_back(&node1, node4);
	printf("node1 = %p\n", node1);
	printf("node2 = %p\n", node2);
	printf("node3 = %p\n", node3);
	printf("node4 = %p\n", node4);
	printf("next1 = %p\n", node1->next);
	printf("next2 = %p\n", node2->next);
	printf("next3 = %p\n", node3->next);
	printf("next4 = %p\n", node4->next);
	lg = ft_lstsize(node1);
	ft_lstiter(node1, to_uppercase_i);
	printf("next1 = %s\n", (char *)node1->content);
        printf("next2 = %s\n", (char *)node2->content);
        printf("next3 = %s\n", (char *)node3->content);
        printf("next4 = %s\n", (char *)node4->content);
	printf("lg = %d\n", lg);
}*/
