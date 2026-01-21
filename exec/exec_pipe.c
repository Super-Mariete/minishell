/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:19:18 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/16 13:20:01 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	wait_children(pid_t *pid, const pid_t last_pid)
{
	int		status;
	int		last_status;
	size_t	nl_put;

	last_status = 0;
	set_sig(IGNORE);
	nl_put = 0;
	*pid = wait(&status);
	while (*pid > 0)
	{
		if (WIFSIGNALED(status) && status == 2 && !nl_put)
			nl_put = write(1, "\n", 1);
		if (*pid == last_pid)
			last_status = status;
		*pid = wait(&status);
	}
	set_sig(PARENT);
	if (WIFSIGNALED(last_status))
		return (printf("HLA\n"), 128 + WTERMSIG(last_status));
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	return (1);
}

static void	manage_fds(const t_cli *cli, int fd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cli->next)
	{
		close(fd[PIPE_WRITE]);
		*prev_fd = fd[PIPE_READ];
	}
}

static void	manage_child_fds(const t_cli *cli, const int *fd, const int prev_fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cli->next)
	{
		dup2(fd[PIPE_WRITE], STDOUT_FILENO);
		close(fd[PIPE_READ]);
		close(fd[PIPE_WRITE]);
	}
}

static bool	manage_fds_at_start(const t_cli *cli, int fd[2], const int prev_fd)
{
	if (cli->next && pipe(fd) < 0)
	{
		if (prev_fd != -1)
			close(prev_fd);
		perror("pipe");
		return (true);
	}
	return (false);
}

int	execute_pipeline(t_cli *cli, pid_t pid, pid_t last_pid)
{
	int		fd[2];
	int		prev_fd;

	prev_fd = -1;
	while (cli)
	{
		if (manage_fds_at_start(cli, fd, prev_fd))
			return (free_env(cli->env), reset_list(cli), 1);
		pid = fork();
		if (pid < 0)
			return (free_env(cli->env), reset_list(cli), perror("fork"), 1);
		if (pid == 0)
		{
			manage_child_fds(cli, fd, prev_fd);
			return (exec_child(cli));
		}
		manage_fds(cli, fd, &prev_fd);
		if (!cli->next)
			last_pid = pid;
		cli = cli->next;
	}
	return (wait_children(&pid, last_pid));
}
