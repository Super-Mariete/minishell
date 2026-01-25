/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/25 19:43:20 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <sys/resource.h>
# include <dirent.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <errno.h>

# define HERE_PIPE 3
# define HERE_DOC 2
# define IGNORE 2
# define PARENT 1
# define CHILD 0
# define READ 2
# define PIPE_READ 0
# define APPEND 1
# define WRITE 0
# define PIPE_WRITE 1
# define SEP_STR " \n\t<>|&()"
# define SEP_STR2 " \n\t<>|\"'"
# define OP_STR "&|()"
# define OP_STR2 "&|"
# define PRNTS "()"

/* Tipos de Operadores */

# define PIPE 1
# define AND 2
# define OR 3
# define OP_PRNTS 4
# define CL_PRNTS 5

# define IFS " \t\n"
# define REDIR_S "<>"
# define QUOTES "\"'"
# define NO_VAL_VAR " !\"#$%&'()*+,-./:;<=>@[]^`{|}~ "
# define ERR_OPEN_Q "minishell: syntax error: quotes not closed\n"
# define ESC_CHARS1 "\\\"?$"
# define PIPE_ERR "minishell: syntax error near unexpected token `|'\n"
# define UNEX_T1 "minishell: syntax error near unexpected token '<'\n"
# define UNEX_T2 "minishell: syntax error near unexpected token '>'\n"
# define HERE_ERR "minishell: warning: here-document \
delimited by end-of-file (wanted `"
# define CMD_ERR ": command not found\n"
# define HERE_PIPE_ERR "minishell: syntax error: unexpected end of file\nexit\n"
# define SYN_ERR "minishell: syntax error near unexpected token `"
# define UNEX_EOF "minishell: syntax error: unexpected end of file\n"
# define MAX_CMDS 64
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

extern volatile sig_atomic_t	g_signal;

typedef struct s_shenv
{
	char			*var;
	struct s_shenv	*next;
}	t_shenv;

typedef struct s_builtin
{
	char	*name;
	int		(*func)(char **args, t_shenv **);
}	t_builtin;

typedef struct s_cli
{
	char			*cmd;
	t_shenv			**env;
	char			**args;
	char			*infile;
	char			*outfile;
	char			*heredoc;
	struct s_cli	*next;
	struct s_cli	*prev;
	size_t			n_tokens;
	int				status;
	int				op;
	int				group;
	int				heredoc_fd;
	bool			is_builtin;
	bool			r_mode;
}	t_cli;

/* minishell.c */

int		check_prnts(const char *line);

/* parsing/lexing.c */

char	**token_sep(char *line, size_t j, size_t i);
int		num_s_tokens(const char *line);
char	**tokenize(const char *line, t_cli *cli);
int		check_errors(char **token, size_t len);
size_t	quoted_len(const char *line);

/* parsing/parsing.c */

int		parse_input(char **tokens, t_cli *cli, size_t i);
t_cli	*parse_op(const char *token, t_cli **cli);
int		set_cmd(char *token, t_cli *cli);
int		add_args(char *token, t_cli *cli, int pos);

/* parsing/parsing_utils*.c */

char	*expand_exit_status(int status, const char *line, size_t i);
bool	expand_t(char ***tokens, size_t *len, size_t *i, int wc_len);
void	reset_free(t_cli *cli);
int		handle_heredoc(t_cli *cli, const int *option,
			const char *delim, const char *line);

/* parsing/utils*.c */

t_cli	*init_node(size_t len, t_shenv **env, int op);
void	free_list(t_cli *cli);
void	free_node(t_cli *cli);
void	free_first_node(t_cli *cli);
void	reset_list(t_cli *cli);
void	print_list(t_cli *cli);
void	free_tokens(char **tokens, size_t n);
void	perror_token(const char *token, const char *msg);
void	perror_msh(const char *problem, const char *mssg);
char	*trim_spaces(const char *line);
t_cli	*parse_prnts(t_cli **cli, char token);

/* parsing/expansion.c */

char	**expand_tokens(char **tokens, size_t *len, const t_cli *cli, size_t i);
char	*expand_line(char *token, const t_cli *cli);
char	*escape_quotes(const char *line);
char	*trim_delim(const char *token, int *option);

/* parsing/wildcards.c */

char	**expand_wildcard(char **token, size_t pos, int *wc_len);
int		init_var(size_t *i, size_t *j, size_t *i_a, size_t *j_after);
int		equ(size_t *j, size_t *i);
int		ft_js(const size_t *j_s, size_t *ia, size_t *i, size_t *j);

/* parsing/heredoc*.c */

int		get_heredoc(const char *token, t_cli *cli);
int		create_heredoc(t_cli *cli);
size_t	heredoc_len(const char *line);
void	here_error(const char *delim);
char	*convert_to_string(char **array);
char	**expand_array(char **array, const t_cli *cli);
size_t	n_quotes(const char *line);

/* parsing/shenv.c */

t_shenv	*load_env(char **envp);
char	**getshenv(const t_shenv *env);
char	*ft_getenv(const t_shenv *ft_env, char *key);
int		set_env(t_shenv **ft_env, char *key, const char *value);
int		unset_env(t_shenv **env, char *key);
void	free_env(t_shenv **ft_env);

/* parsing/signals.c */

void	set_sig(int option);

/* parsing/readline.c */

int		read_input_line(t_cli *cli);
void	process_input(const char *line, t_cli *cli);

/* exec/ft_execute.c */

int		execute(t_cli *cli);
int		execute_pipeline(t_cli *cli, pid_t pid, pid_t last_pid);
int		exec_child(t_cli *cli);
int		(*get_builtin(char *cmd))(char **args, t_shenv **env);
int		execute_command(t_cli *cli);
int		manage_status(t_cli *cli, int status);

/* exec/exec_utils*.c */

bool	create_file(const t_cli *cli);
int		handle_redirs(const t_cli *cli);
int		exec_builtin_child(const t_cli *cli);
int		execute_builtin(t_cli *cli);
void	exec_node(t_cli **cli, int *status);

/* exec_prnts.c */

t_cli	*next_node_pipe(t_cli *cli);
bool	checks_logic(const t_cli *cli);
t_cli	*close_prnts_node(t_cli *cli);
pid_t	handle_prnts(t_cli *cli);

/* exec/aux_exec/apply_redirs.c */

int		apply_redirs(const t_cli *cli);

/* exec/aux_exec/exec_builtin.c */

int		exec_builtin(t_cli *cli, int in, int out);

/* exec/builtins/ *.c */

int		ft_pwd(char **args, t_shenv **ft_env);
int		ft_echo(char **args, t_shenv **env);
int		ft_env(char **args, t_shenv **env);
int		ft_exit(char **args, t_shenv **env);
bool	is_number_str(const char *s);
int		ft_cd(char **args, t_shenv **env);
int		ft_export(char **args, t_shenv **env);
int		ft_unset(char **args, t_shenv **ft_env);

void	print_parser(t_cli *cli);

#endif
