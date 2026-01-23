/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:03:35 by made-ped          #+#    #+#             */
/*   Updated: 2026/01/08 13:04:52 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_access(t_cli *cli)
{
	struct stat	st;

	if (access(cli->cmd, F_OK) != 0)
	{
		perror_msh(cli->cmd, "command not found\n");
		free_env(cli->env);
		reset_free(cli);
		exit(127);
	}
	if (stat(cli->cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		free_env(cli->env);
		perror_msh(cli->cmd, "Is a directory\n");
		reset_free(cli);
		exit(126);
	}
	if (access(cli->cmd, X_OK) != 0)
	{
		free_env(cli->env);
		perror_msh(cli->cmd, "Permission denied\n");
		reset_free(cli);
		exit(126);
	}
}

int	exec_child(t_cli *cli)
{
	set_sig(CHILD);
	if (apply_redirs(cli))
	{
		free_list(cli);
		exit(1);
	}
	if (!cli->cmd)
	{
		reset_list(cli);
		free_env(cli->env);
		exit(0);
	}
	if (get_builtin(cli->cmd))
		exit(exec_builtin_child(cli));
	check_access(cli);
	execve(cli->cmd, cli->args, getshenv(*cli->env));
	perror("minishell: execve");
	free_list(cli);
	exit(126);
}

static int	manage_status(t_cli *cli, const int status)
{
	if (WIFSIGNALED(status))
	{
		if (status == 2)
			write(1, "\n", 1);
		cli->last_status = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		cli->last_status = WEXITSTATUS(status);
	else
		cli->last_status = 1;
	return (cli->last_status);
}

int	execute_command(t_cli *cli)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == CHILD)
		return (exec_child(cli));
	set_sig(IGNORE);
	waitpid(pid, &status, 0);
	set_sig(PARENT);
	ret = manage_status(cli, status);
	return (ret);
}

int	execute(t_cli *cli)
{
	int	status;

	status = 2;
	while (cli)
	{
		if (checks_logic(cli))
		{
			if (!cli->cmd && cli->op != CL_PRNTS && cli->op != OP_PRNTS)
			{
				if (cli->heredoc || cli->infile || cli->outfile)
					cli->last_status = handle_redirs(cli);
				else
				{
					perror_msh(NULL, "command not found\n");
					cli->last_status = 2;
				}
			}
			else if (cli->next != NULL && cli->next->op == PIPE)
			{
				execute_pipeline(cli, -1, -1);
				cli = next_node_pipe(cli);
			}
			else if (cli->is_builtin)
				execute_builtin(cli);
			else
				execute_command(cli);
			status = cli->last_status;
		}
		cli = cli->next;
		if (cli)
			cli->last_status = cli->prev->last_status;
	}
	return (status);
}

