/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/05 19:36:41 by rafael           ###   ########.fr       */
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
# define METACHARS " \t\n|&()<>" //Omited ';'
# define CONTROL_OP "<>&|()"
# define OP "&|()"
# define PRTS "()"
# define PROMPT "\033[32mminishell\033[0m $ "
# define QUOTES "\"\'"
# define REDIR_S "<>"

// If PATH not in environment (predetermined PATH, almost sure invalid)
# define PATH "/bin:/sbin/:local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin"

// Set up signal handling status
# define PARENT 0
# define CHILD 1
# define IGNORE 2

// Error messages
# define MEMOUT "minishell: buffer out of memory\n"
# define ARG2BIG "minishell: Argument list too long\n"
# define NOSTASH "\nminishell: MEMOUT: can't stash current line\n"
# define UNEXPTKN "minishell: syntax error near unexpected token "
# define UNCLOSED "minishell: expected another "
# define NOARGS "minishell: expected arguments after last "

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
}	t_env;

// Struct for managing history
typedef struct s_hist
{
	char	*buffer;
	char	*last;
	char	*current;
	char	*stash;
	size_t	line_len;
}	t_hist;

// Struct for readline buffer data
typedef struct s_read
{
	char	*buffer;
	t_hist	*hist;
	size_t	cursor;
	size_t	line_len;
	int		intr;
	int		quoted;
}	t_read;

// AST node types
typedef	enum
{
	CMD = 1,
	PIPE,
	OPEN_PRTS,
	CLS_PRTS,
	OR,
	AND
}	node_type;

// Struct for each node of the AST
typedef struct s_node
{
	struct s_node	*prev;
	struct s_node	*left;
	struct s_node	*right;
	char			*cmd;
	char			*heredoc;
	char			*infile;
	char			*outfile;
	char			*args;
	int				type;
	int				append;
	int				n_args;
	int				n_node;
	int				is_builtin;
	
}	t_node;

// Struct for the AST
typedef struct s_ast
{
	t_node	*first;
	t_node	*current;
	int		n_nodes;
}	t_ast;

// Buffer holding the lexed line
typedef struct s_buffer
{
	char	*buffer;
	char	*current;
	char	*last;
}	t_buffer;

// Struct with references to the most used structures in the program
typedef struct s_msh
{
	t_term		*term; // Maybe not necessary
	t_env		*env;
	t_read		*rbuffer;
	t_buffer	*buff;
	t_ast		*ast;
	int			*status;
}	t_msh;

/* signals */

void	ft_set_sig(int option);

/* variables */

int		ft_load_env(t_env *env, char **envp);

/* readline */

int		ft_init_term(t_term *term);
int		read_key(char *c);
int		ft_readline(t_msh *msh);
int		ft_process_key(const char c, t_read *rbuffer, t_msh *msh);
int		ft_process_arrows(t_read *rbuffer);
int		ft_process_nl(t_read *rbuffer, t_msh *msh);
void	ft_reset_read(t_read *rbuffer);
void	ft_up_history(t_read *rbuffer);
void	ft_down_history(t_read *rbuffer);
void	ft_add_to_history(t_read *rbuffer);
void	ft_reset_cl(const t_read *rbuffer);
void	ft_reset_cursor(const t_read *rbuffer);
void	ft_reset_read(t_read *rbuffer);
char	*ft_get_stash(const t_read *rbuffer);

//Printers for debugging
void	ft_print_history(t_hist *hist);
void	ft_print_ast(t_ast *ast);

/* parse */

size_t	ft_parse(t_msh *msh);
size_t	ft_lexer(t_msh *msh);
void	ft_reset_buffer(t_buffer *buff);
size_t	ft_quoted_len(const char *line, char quote);
size_t	ft_op_len(const char *line, int pos);
size_t	ft_next_token(const char *line, size_t pos);

/* utils */

size_t	ft_buffercpy(const char *src, char *dest, size_t size);
void	ft_perror_token(const char token, const char *error);
void	ft_perror_str_token(const char *token, const char *error);

// void	ft_refill_var_buffer(char *buffer, t_env *env);

/* exec */

void	ft_unset(t_msh *msh, char *key);

#endif
