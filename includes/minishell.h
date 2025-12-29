/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/29 21:50:18 by rafael           ###   ########.fr       */
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
# include <stdbool.h>
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
# define PROMPT "\033[32mminishell\033[0m $ "

// If PATH not in environment (predetermined PATH)
# define PATH "/bin:/sbin/:local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin"

// Set up signal handling status
# define PARENT 0
# define CHILD 1
# define IGNORE 2

// Error messages
# define MEMOUT "minishell: buffer out of memory\n"
# define ARG2BIG "minishell: Argument list too long\n"

// Only allowed variable, to catch signals
extern sig_atomic_t	g_signal;

// Terminal relevant data for readline
typedef struct s_term
{
	struct termios	canon_mode;
	struct termios	raw_mode;
	int				term_rows;
	int				term_cols;
}	t_term;

// List of local and environment (if exported) variables
typedef struct s_env
{
	char			*head;
	char			*last;
	char			*arena;
	char			*cursor;
	struct s_env	*next;
}	t_env;

// Struct for readline buffer data
typedef struct s_read
{
	unsigned char	*buffer;
	size_t			cursor;
	size_t			line_len;
	unsigned char	*history;
}	t_read;

// Struct with references to the most used structures in the program
typedef struct s_msh
{
	t_term	*term; // Maybe not necessary
	t_env	*env;
	t_read	*read;
	char	*cmd_env;
	char	*cmd_cursor;
	int		status;
}	t_msh;

// Linked list for the commands environment
typedef struct s_cmd
{
	t_msh	*msh;
	char	*cmd;
	char	**argv;
	char	*heredoc;
	char	*input;
	char	*output;
	int		mode;
}	t_cmd;

/* signals */

void	ft_set_sig(int option);

/* init */

int		ft_load_env(t_env *env, char **envp);

/* readline */
int		ft_init_term(t_term *term);
int		read_key(unsigned char *c);
int		ft_readline(t_msh *msh);
int		ft_process_key(const unsigned char c, t_read *read);
int		ft_process_printable(const unsigned char c, t_read *read);
int		ft_process_arrows(t_read *read);
void	ft_reset_buffer(t_read *read);

/* parse */

/* utils */
size_t	ft_buffercpy(const char *src, char *dest, size_t size);
// void	ft_refill_var_buffer(char *buffer, t_env *env);

/* exec */
void	ft_unset(t_msh *msh, char *key);

#endif
