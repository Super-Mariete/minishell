/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:16 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:17:18 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_rec = 0;

int	ft_check_prnts(char *line)
{
	int		i;
	int		len;
	int		prnts;

	if (!line)
		return (-1);
	i = 0;
	prnts = 0;
	while (i < ft_strlen(line))
	{
		if (ft_strchr(QUOTES, line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			len = ft_quoted_len(line + i, line[i]);
			if (len < 0)
				return (-1);
			i += (len - 1);
		}
		if (line[i] == '(')
			prnts++;
		if (line[i] == ')')
			prnts--;
		i++;
	}
	if (prnts)
		write(2, "minishell : extra parenthesis\n", 30);
	return (prnts);
}

void	ft_reset_list(t_cli *cli)
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
		ft_free_list(&next);
		cli->next = nullptr;
	}
	free(cli->cmd);
	cli->cmd = nullptr;
	free(cli->heredoc);
	cli->heredoc = nullptr;
	free(cli->infile);
	cli->infile = nullptr;
	free(cli->outfile);
	cli->outfile = nullptr;
	ft_free_tokens(cli->args, cli->n_tokens - 1);
	cli->args = nullptr;
	cli->is_builtin = 0;
	cli->r_mode = 0;
	cli->group = 0;
	cli->op = 0;
}

int	ft_reset_signal(t_cli *cli)
{
	g_sig_rec = 0;
	ft_reset_list(cli);
	cli->last_status = 130;
	return (1);
}

int	ft_read_line(t_shenv **env, t_cli *cli)
{
	char	*cl;
	char	**tokens;

	cl = NULL;
	while (1)
	{
		free(cl);
		printf("status = %d\n", cli->last_status);
		cl = readline("\033[1;32mminishell\033[0m$ ");
		if (!cl)
			return (rl_clear_history(), write(1, "exit\n", 5), 0);
		if (g_sig_rec && ft_reset_signal(cli))
			continue ;
		add_history(cl);
		tokens = ft_tokens(cl, *env, cli);
		if (!tokens)
		{
			cli->last_status = 2;
			continue ;
		}
		cli->status = ft_parse(tokens, cli);
		cli->status = ft_execute(cli);
		cli->last_status = cli->status;
		ft_reset_list(cli);
	}
	// return (free(cl), rl_clear_history(), cli->last_status);
}

int	ft_event_hook(void)
{
	if (g_sig_rec)
		rl_done = 1;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shenv		*env;
	extern int 	rl_catch_signals;
	t_cli		*cli;
	int			status;
	
	ft_set_sig(PARENT);
	rl_catch_signals = 0;
	rl_event_hook = ft_event_hook;
	env = ft_load_env(envp);
	cli = ft_init_node(1, &env, 0);
	if (!cli)
		return (ft_free_env(&env), 2);
	status = ft_read_line(&env, cli);
	ft_free_list(&cli);
	ft_free_env(&env);
	return (status);
}
