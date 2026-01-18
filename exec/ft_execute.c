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
		reset_free(cli);
		free_env(cli->env);
		exit(1);
	}
	if (!cli->cmd)
	{
		reset_free(cli);
		free_env(cli->env);
		exit(0);
	}
	if (get_builtin(cli->cmd))
		exit(exec_builtin_child(cli));
	check_access(cli);
	execve(cli->cmd, cli->args, getshenv(*cli->env));
	perror("execve");
	free_env(cli->env);
	reset_free(cli);
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

static int	execute_command(t_cli *cli)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == CHILD)
		return (exec_child(cli));
	set_sig(IGNORE);
	waitpid(pid, &status, 0);
	set_sig(PARENT);
	return (manage_status(cli, status));
}

int	execute(t_cli *cli)
{
	int	piped;
	int	ret;

	if (!cli)
		return (2);
	if (!cli->cmd)
	{
		if (cli->heredoc || cli->infile || cli->outfile)
			return (handle_redirs(cli));
		reset_list(cli);
		return (perror_msh(NULL, "command not found\n"), 2);
	}
	piped = has_pipe(cli);
	if (get_builtin(cli->cmd) && !piped)
	{
		ret = execute_builtin(cli);
		return (reset_list(cli), ret);
	}
	if (piped)
		return (execute_pipeline(cli, -1, -1));
	return (execute_command(cli));
}
