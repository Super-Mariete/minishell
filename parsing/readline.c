#include "../minishell.h"

static void	free_first_node(t_cli *cli)
{
	free(cli->cmd);
	cli->cmd = nullptr;
	free(cli->heredoc);
	cli->heredoc = nullptr;
	free(cli->infile);
	cli->infile = nullptr;
	free(cli->outfile);
	cli->outfile = nullptr;
	ft_free_d(cli->args);
	cli->args = nullptr;
	cli->is_builtin = 0;
	cli->r_mode = 0;
	cli->group = 0;
	cli->op = 0;
}

void	reset_list(t_cli *cli)
{
	t_cli	*next;
	t_cli	*last;

	if (!cli)
		return ;
	last = cli;
	while (last->next)
		last = last->next;
	cli->status = last->status;
	next = cli->next;
	if (next)
	{
		free_list(&next);
		cli->next = nullptr;
	}
	free_first_node(cli);
}

int	reset_signal(t_cli *cli)
{
	g_signal = 0;
	reset_list(cli);
	cli->last_status = 130;
	return (1);
}

static int	is_empty(char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (!ft_isspace(*s))
			return (0);
		s++;
	}
	return (1);
}

int	read_input_line(t_shenv **ft_env, t_cli *cli)
{
	char	*cl;
	char	**tokens;

	cl = nullptr;
	while (1)
	{
		free(cl);
		cl = readline("\033[1;32mminishell\033[0m$ ");
		if (!cl)
			return (rl_clear_history(), write(1, "exit\n", 5), 2);
		if  ((g_signal && reset_signal(cli)) || is_empty(cl))
			continue ;
		add_history(cl);
		tokens = tokenize(cl, *ft_env, cli);
		if (!tokens)
		{
			cli->last_status = 2;
			continue ;
		}
		cli->status = parse_input(tokens, cli);
		cli->status = execute(cli);
		cli->last_status = cli->status;
		reset_list(cli);
	}
}