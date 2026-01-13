#include "../minishell.h"

void	ft_exec(t_cli *cli)
{
	if (cli->status != 130)
	{
		cli->status = execute(cli);
		cli->last_status = cli->status;
	}
	reset_list(cli);
}
