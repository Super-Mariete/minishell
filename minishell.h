/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/07 19:08:26 by made-ped         ###   ########.fr       */
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
# define SEP_STR2 " \n\t<>|\"\'"
# define OP_STR "&|()"
# define OP_STR2 "&|"
# define PRNTS "()"
# define PIPE 1
# define AND 2
# define OR 3
# define IFS " \t\n"
# define REDIR_S "<>"
# define QUOTES "\"\'"
# define NO_VAL_VAR " !\"#$%&'()*+,-./:;<=>@[]^`{|}~ "
# define ERR_OPEN_Q "minishell: syntax error: quotes not closed\n"
# define ESC_CHARS1 "\\\"\?$"
# define PIPE_ERR "minishell: syntax error near unexpected token `|'\n"
# define UNEX_T1 "minishell: syntax error near unexpected token '<'\n"
# define UNEX_T2 "minishell: syntax error near unexpected token '>'\n"
# define HERE_ERR "minishell: warning: here-document\
elimited by end-of-file (wanted `"
# define CMD_ERR ": command not found\n"
# define HERE_PIPE_ERR "minishell: syntax error: unexpected end of file\nexit\n"
# define SYN_ERR "minishell: syntax error near unexpected token `"
# define UNEX_EOF "minishell: syntax error: unexpected end of file\n"
# define MAX_CMDS 64
# ifndef PATH_MAX
# define PATH_MAX 4096
# endif

//solo por la compatibilidad con mac, luego se elimina
# ifndef rl_clear_history
#  define rl_clear_history() clear_history()
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
	char			**args;
	char			*infile;
	char			*outfile;
	char			*heredoc;
	int				heredoc_fd;
	int				is_builtin;
	int				r_mode;
	t_shenv			**ft_env;
	int				n_tokens;
	int				status;
	int				last_status;
	int				group;
	int				op;
	struct s_cli	*next;
}	t_cli;

bool	has_pipes_or_redirs(t_cli *cli);
char	**expand_wildcard(char **token, int pos, int *wc_len);
char	**token_sep(char *line);
char	**insert_s_tokens(char **tokens);
char	**lex_pipe(char **token, int *len);
char	**expand_tokens(char **tokens, int *len, const t_cli *cli);
char	**tokenize(char *line, t_shenv *ft_env, t_cli *cli);
char	**getshenv(t_shenv *ft_env);
char	*prompt(char **envp);
char	*get_hostname(void);
char	*get_pwd(char *cwd);
char	*expand_line(char *token, const t_cli *cli);
char	*expand_var(char *token, size_t start, size_t end);
char	*get_var(char *var_call, char **envp);
char	*escaped_line(char *line, int start, int end);
char	*escape_quotes(char *line);
char	*trim_delim(char *token, int *option);
char	*expand_heredoc(int option, t_cli *cli);
char	*cmd_path(char *env_path, char *cmd);
char	*ft_getenv(t_shenv *ft_env, char *key);
char	*expand_exit_status(int status, const char *line, size_t i);
int		ft_export(char **args, t_shenv **ft_env);
int		ft_unset(char **args, t_shenv **ft_env);
int		unset_env(t_shenv **ft_env, char *key);
int		init_var(size_t *i, size_t *j, size_t *i_a, size_t *j_after);
int		ft_equal(size_t *j, size_t *i);
int		ft_j_s(const size_t *j_s, size_t *i_a, size_t *i, size_t *j);
int		match_wildcard(const char *str, const char *wildcard);
int		parse_input(char **tokens, t_cli *cli);
int		check_prnts(char *line);
int		check_errors(char **token, int len);
int		ft_pwd(char **args, t_shenv **ft_env);
int		ft_echo(char **args, t_shenv **ft_env);
int		ft_env(char **args, t_shenv **ft_env);
int		ft_exit(char **args, t_shenv **ft_env);
int		ft_cd(char **args, t_shenv **ft_env);
int		set_env(t_shenv **ft_env, char *key, char *value);
int		execute_command(t_cli *cli);
int		execute_builtin(t_cli *cmd);
int		execute(t_cli *cli);
int		execute_pipeline(t_cli *cli);
int		sep_len(char *line, int pos);
int		num_quoted(char *line);
int		quoted_len(const char *line, char quote);
int		token_len(char *line);
int		read_input_line(t_shenv **ft_env, t_cli *cli);
int		num_s_tokens(char *line);
size_t	var_len(char	*var);
int		trim_s_len(char *line);
int		append(char *token, t_cli *cli);
int		heredoc_len(const char *line);
int		heredoc(char *token, t_cli *cli);
int		infile(char *token, t_cli *cli);
int		outfile(char *token, t_cli *cli);
int		set_cmd(char	*token, t_cli *cli);
int		add_args(char *token, t_cli *cli, int pos);
int		shenv_len(t_shenv *ft_env);
int		(*get_builtin(char *cmd))(char **, t_shenv **);
void	set_sig(int option);
void	sig_parent(int signal);
void	free_list(t_cli **cli);
void	free_node(t_cli *cli);
void	here_error(char *delim);
void	free_tokens(char **tokens, int n);
void	perror_token(char *token, char *msg);
void	perror_msh(char *problem, char *mssg);
void	free_env(t_shenv **ft_env);
void	reset_list(t_cli *cli);
t_cli	*init_node(int len, t_shenv **envp, int op);
t_cli	*parse_op(char *token, t_cli *cli);
t_shenv	*load_env(char **envp);
void	print_list(t_cli *cli);
char	*trim_spaces(char *line);

int		has_pipe(t_cli *cli);
int		apply_redirs(t_cli *cli);
int		exec_builtin(t_cli *cli);

#ifdef _APPLE_
int	rl_catch_signals = 0;
//solo para aple, después eliminar
#endif

#endif
