/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/22 13:44:50 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Just to shut up IntelliSense in VS Code
# define _GNU_SOURCE

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft/libft.h"
// // # include <fcntl.h>
// # include <sys/resource.h>
# include <dirent.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
// # include <stdbool.h>
# include <errno.h>
# include <termios.h>
# include "msh_limits.h"

# define IFS " \t\n"
# define METACHARS " \t\n|&;()<>"
# define CONTROL_OP "<>&|"
# define PROMPT "\e[32mminishell\e[0m $ "
// Max size in bytes of char **argv + char **envp for execve

// Set up signal handling status
# define PARENT 0
# define CHILD 1
# define IGNORE 2

// Only allowed variable, to catch signals
extern sig_atomic_t	g_signal;

// Terminal relevant data for readline
typedef struct s_term
{
	struct termios	canon_mode;
	struct termios	raw_mode;
	char			*buffer;
	int				cursor;
	int				term_rows;
	int				term_cols;
	size_t			line_len;
	char			*history;
}	t_term;

// List of local and environment (if exported) variables
typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_exported;
	struct s_env	*next;
}	t_env;

/* signals */

void	ft_set_sig(int option);

/* init */

int		ft_init_var_pool(t_env *pool);

/* readline */

/* parse */

#endif
