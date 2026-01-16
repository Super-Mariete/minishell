/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 10:09:47 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/13 11:11:14 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_var(size_t *i, size_t *j, size_t *i_a, size_t *j_after)
{
	*i = 0;
	*j = 0;
	*i_a = 0;
	*j_after = 0;
	return (1);
}

int	equ(size_t *j, size_t *i)
{
	*j = *j + 1;
	*i = *i + 1;
	return (1);
}

int	ft_js(const size_t *j_s, size_t *ia, size_t *i, size_t *j)
{
	*ia = *ia + 1;
	*i = *ia;
	*j = *j_s;
	return (1);
}
