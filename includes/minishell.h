/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rms35 <rms35@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:26 by rafael-m          #+#    #+#             */
/*   Updated: 2025/10/13 21:27:22 by rms35            ###   ########.fr       */
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
# include <stdbool.h>
# include <errno.h>

# define HERE_PIPE 3
# define HERE_DOC 2
# define IGNORE 2
# define PARENT 1
# define CHILD 0
# define READ 2
# define APPEND 1
# define WRITE 0
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
# define HERE_ERR "minishell: warning: here-document delimited by end-of-file (wanted `"
# define CMD_ERR ": command not found\n"
# define HERE_PIPE_ERR "minishell: syntax error: unexpected end of file\nexit\n"
# define SYN_ERR "minishell: syntax error near unexpected token `"
# define UNEX_EOF "minishell: syntax error: unexpected end of file\n"
# define MAX_CMDS 64
# ifndef PATH_MAX
# define PATH_MAX 4096
# endif

extern volatile sig_atomic_t	g_sig_rec;

typedef	struct s_shenv
{
	char	*var;
	struct s_shenv	*next;
}	t_shenv;

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
	t_shenv			**env;
	int				n_tokens;
	int				status;
	int				last_status;
	int				group;
	int				op;
	struct s_cli	*next;
}	t_cli;

bool 	has_pipes_or_redirs(t_cli *cli);
char	**ft_expand_wildcard(char **token, int pos, int *wc_len);
char	**ft_token_sep(char *line);
char	**ft_insert_s_tokens(char **tokens);
char	**ft_lex_pipe(char **token, int *len);
char	**ft_expand_tokens(char **tokens, int *len, t_cli *cli);
char	**ft_tokens(char *line, t_shenv *env, t_cli *cli);
char	**ft_getshenv(t_shenv *env);
char	*ft_prompt(char **envp);
char	*get_hostname(void);
char	*get_pwd(char *cwd);
char	*ft_expand_line(char *token, t_cli *cli);
char	*ft_expand_var(char	*token, int start, int end);
char	*ft_get_var(char *var_call, char **envp);
char	*ft_escaped_line(char *line, int start, int end);
char	*ft_escape_quotes(char *line);
char	*ft_trim_delim(char *token, int *option);
char	*ft_expand_heredoc(int option, t_cli *cli);
char    *ft_cmd_path(char *env_path, char *cmd);
char 	*ft_getenv(t_shenv *env, const char *key);
char	*ft_expand_exit_status(int status, char *line, int i);
int 	ft_export(char **args, t_shenv **env);
int 	ft_unset(char **args, t_shenv **env);
int 	ft_unsetenv(t_shenv **env, const char *key);
int		ft_init_var(size_t *i, size_t *j, size_t *i_a, size_t *j_after);
int		ft_equal(size_t *j, size_t *i);
int		ft_j_s(size_t *j_s, size_t *i_a, size_t *i, size_t *j);
int		ft_match_wildcard(char *str, char *wildcard);
int		ft_parse(char **tokens, t_cli *cli);
int		ft_check_prnts(char *line);
int		ft_check_errors(char **token, int len);
int 	ft_pwd(char **args);
int 	ft_echo(char **args);
int 	ft_env(char **env);
int 	ft_exit(void);
int 	ft_cd(char **args, t_shenv **env);
int		ft_setenv(t_shenv **env, const char *key, const char *value);
int 	execute_command(t_cli *cli);
int 	execute_builtin(t_cli *cmd);
int 	ft_execute(t_cli *cli);
int 	execute_pipeline(t_cli *cli);
int		ft_sep_len(char *line, int pos);
int		ft_num_quoted(char *line);
int		ft_quoted_len(char *line, char quote);
int		ft_token_len(char *line);
int		ft_read_line(t_shenv **env, t_cli *cli);
int		ft_num_s_tokens(char *line);
int		ft_var_len(char	*var);
int 	ft_trim_s_len(char *line);
int		ft_append(char *token, t_cli *cli);
int		ft_heredoc_len(char *line);
int		ft_heredoc(char *token, t_cli *cli);
int		ft_infile(char *token, t_cli *cli);
int		ft_outfile(char *token, t_cli *cli);
int		ft_cmd(char	*token, t_cli *cli);
int		ft_args(char *token, t_cli *cli, int pos);
int		ft_shenv_len(t_shenv *env);
void	ft_set_sig(int option);
void	ft_sig_parent(int signal);
void	ft_free_list(t_cli **cli);
void	ft_free_node(t_cli *cli);
void	ft_here_error(char *delim);
void	ft_free_tokens(char **tokens, int n);
void	ft_perror(char *token, char *msg);
void	ft_free_env(t_shenv **env);
void	ft_reset_list(t_cli *cli);
t_cli	*ft_init_node(int len, t_shenv **envp, int op);
t_cli	*ft_parse_op(char *token, t_cli *cli);
t_shenv	*ft_load_env(char **envp);
void	ft_print_list(t_cli *cli);
char	*ft_trim_spaces(char *line);

#endif
