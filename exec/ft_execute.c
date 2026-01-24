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
		free_list(cli);
		exit(127);
	}
	if (stat(cli->cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		perror_msh(cli->cmd, "Is a directory\n");
		free_list(cli);
		exit(126);
	}
	if (access(cli->cmd, X_OK) != 0)
	{
		perror_msh(cli->cmd, "Permission denied\n");
		free_list(cli);
		exit(126);
	}
}

int	exec_child(t_cli *cli)
{
	int	status;

	set_sig(CHILD);
	if (apply_redirs(cli))
	{
		free_list(cli);
		exit(1);
	}
	if (!cli->cmd)
	{
		free_list(cli);
		exit(0);
	}
	if (cli->is_builtin)
	{
		status = exec_builtin_child(cli);
		free_list(cli);
		exit(status);
	}
	check_access(cli);
	execve(cli->cmd, cli->args, getshenv(*cli->env));
	perror("minishell: execve");
	free_list(cli);
	exit(126);
}

int	manage_status(t_cli *cli, const int status)
{
	if (WCOREDUMP(status))
	{
		if (status == 134)
			write(2, "Aborted                    ", 27);
		if (status == 136)
			write(2, "Floating point exception   ", 27);
		write(2, "(core dumped) ", 14);
		write(2, cli->cmd, ft_strlen(cli->cmd));
		write(2, "\n", 1);
		cli->status = 136;
	}
	if (WIFSIGNALED(status))
	{
		if (status == 2)
			write(1, "\n", 1);
		cli->status = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		cli->status = WEXITSTATUS(status);
	else
		cli->status = 1;
	return (cli->status);
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
		return (cli->status = 2, 1);
	}
	if (pid == CHILD)
		return (exec_child(cli));
	set_sig(IGNORE);
	waitpid(pid, &status, 0);
	set_sig(PARENT);
	ret = manage_status(cli, status);
	return (cli->status = ret, ret);
}

int	execute(t_cli *cli)
{
	int	status;

	status = 2;
	while (cli)
	{
		if (!checks_logic(cli) && cli->op == OP_PRNTS && printf("HEY!\n"))
			cli = close_prnts_node(cli);
		else if (checks_logic(cli))
		{
			exec_node(&cli, &status);
			status = cli->status;
		}
		cli = cli->next;
		if (cli)
			cli->status = cli->prev->status;
	}
	return (status);
}
