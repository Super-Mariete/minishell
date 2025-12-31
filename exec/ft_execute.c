/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:03:35 by made-ped          #+#    #+#             */
/*   Updated: 2025/12/31 12:38:19 by made-ped         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// Busca si el comando es un builtin y devuelve su función si existe
int (*get_builtin(char *cmd))(char **, t_shenv **)
{
	static const t_builtin builtins[] = {
		{"echo", ft_echo},
		{"pwd", ft_pwd},
		{"cd", ft_cd},
		{"export", ft_export},
		{"unset", ft_unset},
		{"env", ft_env},
		{"exit", ft_exit},
		{NULL, NULL}
	};

	int i = 0;
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
			return (builtins[i].func);
		i++;
	}
	return (NULL);
}
/*
int	ft_execute(t_cli *cli)
{
	int	stdin_save;
	int	stdout_save;
	int	status;
//	int (*builtin)(char **, t_shenv **); // ← ahora tiene punto y coma y tipo correcto

	if (!cli || !cli->cmd)
		return (cli->last_status);

//	builtin = get_builtin(cli->cmd);
//	if (builtin)
//		return (builtin(cli->args, cli->env));
	if (get_builtin(cli->cmd) && !has_pipe(cli))
	{
		stdin_save = dup(STDIN_FILENO);
		stdout_save = dup(STDOUT_FILENO);

		if (apply_redirs(cli))
			return(1);
		status = exec_builtin(cli);
		dup2(stdin_save, STDIN_FILENO);
		dup2(stdout_save, STDOUT_FILENO);
		close (stdin_save);
		close(stdout_save);
		cli->last_status = status;
		return(status);
	}
	//TODO enteros y pipes;
	return (0);
}
*/

int	ft_execute(t_cli *cli)
{
	if(!cli || !cli->cmd)
		return (cli->last_status);
	if(get_builtin(cli->cmd) && !has_pipe(cli))
		return (execute_builtin(cli));
	if(has_pipe(cli))
		return (execute_pipeline(cli));
	return (execute_command(cli));
}

int	execute_builtin(t_cli *cli)
{
	int stdin_save;
	int stdout_save;
	int status;

	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if(apply_redirs(cli))
		return (1);
	status = exec_builtin(cli);
	dup2(stdin_save, STDIN_FILENO);
	dup2(stdout_save, STDOUT_FILENO);
	close(stdin_save);
	close(stdout_save);
	cli->last_status = status;
	return (status);
}

int	exec_builtin_child(t_cli *cli)
{
	int (*builtin)(char **, t_shenv **);

	builtin = get_builtin(cli->cmd);
	if (!builtin)
		return (1);
	return (builtin(cli->args, cli->env));
}

int execute_command(t_cli *cli)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return(1);
    }
    if (pid == CHILD)
    {
        ft_set_sig(CHILD);
        if (apply_redirs(cli))
            exit(1);

        // Usar directamente cli->cmd
        if (access(cli->cmd, X_OK) != 0)
        {
            ft_perror_msh(cli->cmd, "command not found\n");
            exit(127);
        }
        execve(cli->cmd, cli->args, ft_getshenv(*cli->env));
        perror("execve");
        exit(126);
    }
    ft_set_sig(PARENT);
    waitpid(pid, &status, 0);
    cli->last_status = WEXITSTATUS(status);
    return (cli->last_status);
}

int execute_pipeline(t_cli *cli)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    pid_t last_pid = -1;
    int status;
    int last_status = 0;

    while (cli)
    {
        if (cli->next && pipe(fd) < 0)
            return (perror("pipe"), 1);
        
        pid = fork();
        if (pid < 0)
            return (perror("fork"), 1);
            
        if (pid == 0)
        {
            ft_set_sig(CHILD);
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
            if (apply_redirs(cli))
                exit(1);
            if (get_builtin(cli->cmd))
                exit(exec_builtin_child(cli));

            // Usar directamente cli->cmd que ya tiene el path del parsing
            if (access(cli->cmd, X_OK) != 0)
            {
                ft_perror_msh(cli->cmd, "command not found\n");
                exit(127);
            }
            execve(cli->cmd, cli->args, ft_getshenv(*cli->env));
            perror("execve");
            exit(126);
        }
        
        if (prev_fd != -1)
            close(prev_fd);
        if (cli->next)
        {
            close(fd[PIPE_WRITE]);
            prev_fd = fd[PIPE_READ];
        }
        if (!cli->next)
            last_pid = pid;
        cli = cli->next;
    }
    
    while ((pid = wait(&status)) > 0)
    {
        if (pid == last_pid)
            last_status = status;
    }
    return (WEXITSTATUS(last_status));
}
