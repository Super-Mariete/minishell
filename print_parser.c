
static void	print_op(int op)
{
	if (op == PIPE)
		printf("PIPE (|)\n");
	else if (op == AND)
		printf("AND (&&)\n");
	else if (op == OR)
		printf("OR (||)\n");
	else if (op == OP_PRNTS)
		printf("OP_PRNTS (()\n");
	else if (op == CL_PRNTS)
		printf("CL_PRNTS ())\n");
	else
		printf("NONE\n");
}

static void     print_args(char **args)
{
	int             i;

	printf("Args: [");
	if (args)
	{
		i = 0;
		while (args[i])
		{
			printf("'%s'", args[i]);
			if (args[i + 1])
				printf(", ");
			i++;
		}
	}
	printf("]\n");
}
void    print_parser(t_cli *cli)
{
	t_cli   *node;
	int             index;

	if (!cli)
		return ;
	node = cli;
	index = 0;
	while (node)
	{
		printf("--- Node %d ---\n", index++);
		printf("Op: ");
		print_op(node->op);
		printf("Cmd: %s\n", node->cmd ? node->cmd : "(null)");
		print_args(node->args);
		printf("Infile: %s\n", node->infile ? node->infile : "(null)");
		printf("Outfile: %s\n", node->outfile ? node->outfile : "(null)");
		printf("Heredoc: %s\n", node->heredoc ? node->heredoc : "(null)");
		printf("R_mode: %d\n", node->r_mode);
		printf("Is_builtin: %d\n", node->is_builtin);
		node = node->next;
	}
	printf("--- End of List ---\n");
}
