/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_limits.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:42:57 by rafael            #+#    #+#             */
/*   Updated: 2026/01/06 10:25:57 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO: get limits from OS?
#ifndef MSH_LIMITS_H
# define MSH_LIMITS_H

# ifndef ARG_MAX
#  define ARG_MAX 20480//2097152
# endif

# ifndef CMD_MAX
#  define CMD_MAX 682
# endif

# ifndef BUF_MAX
#  define BUF_MAX 2048
# endif

# ifndef HIST_MAX
#  define HIST_MAX 2097152
# endif

# ifndef CMDENV_MAX
#  define CMDENV_MAX 2048
# endif

# ifndef MAX_NODES
#  define MAX_NODES 1024
# endif

#endif