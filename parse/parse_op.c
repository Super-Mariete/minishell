#include "../includes/minishell.h"

static size_t	ft_parse_cls_prnts(t_ast *ast)
{
	// ast->current = ast->current->prev;
	// ast->n_nodes++;
	// ast->current->prev = (ast->n_nodes * sizeof(t_node));
	// ast->n_nodes++;
	// ast->current->right = (ast->n_nodes * sizeof(t_node));
	// ast->current = ast->
	printf("%p\n", (void *)ast);
	return (0);
}

static size_t	ft_parse_op_prnts(t_ast *ast)
{
	printf("%p\n", (void *)ast);
	// if (ast->n_nodes + 3 >= MAX_NODES)
	// 	return (write(2, NONODES, sizeof(NONODES)), SIZE_MAX - 1);
	// ast->current->type = OPEN_PRTS;
	// ast->n_nodes++;
	// ast->current->right = ast->first + (ast->n_nodes * sizeof(t_node));
	// ast->current->right->type = CLS_PRTS;
	// ast->n_nodes++;
	// ast->current->left = ast->first + (ast->n_nodes * sizeof(t_node));
	// ast->current = ast->current->left;
	// ast->n_nodes++;
	// ast->current->right = ast->first + (ast->n_nodes * sizeof(t_node));
	// ast->current = ast->current->right;
	return (1);
}

/* Create prev node, if !prev node, or prev->prev if prev node, and prev right node, 
setting us at prev node */
static size_t	ft_setup_nodes(t_ast *ast)
{
	printf("current node = %p\n", (void *)ast->current);
	if (ast->n_nodes + 2 >= MAX_NODES)
		return (SIZE_MAX - 1);
	if (!ast->current->prev)
	{
		ast->n_nodes++;
		ast->current->prev = ast->first + (ast->n_nodes * sizeof(t_node));
		ast->current = ast->current->prev;
	}
	else
	{
		ast->n_nodes++;
		ast->current->prev->prev = ast->first + (ast->n_nodes * sizeof(t_node));
		ast->current = ast->current->prev->prev;
	}
	printf("new_op_node = %p\n", (void *)ast->current);
	ast->n_nodes++;
	ast->current->right = ast->first + (ast->n_nodes * sizeof(t_node));
	ast->current->right->prev = ast->current;
	printf("new_op_node->right = %p\n", (void *)ast->current->right);
	return (0);
}	

size_t	ft_parse_op(char *buff, size_t pos, t_ast *ast)
{
	size_t	ret;
	size_t	next;

	printf("op!\n");
	next = ft_next_token(buff, pos);
	if (buff[next] && ft_strchr(OP, buff[next]))
	{
		ft_perror_str_token(&(buff[next]), UNEXPTKN);
		return (SIZE_MAX - 1);
	}
	if (ft_setup_nodes(ast))
		return (SIZE_MAX - 1);
	ret = 2;
	if (buff[pos] == '|' && buff[pos + 1] == '|')
		ast->current->type = OR;
	else if (buff[pos] == '|' )
	{
		ast->current->type = PIPE;
		ret = 1;
	}
	else if (buff[pos] == '&' && buff[pos + 1] == '&')
		ast->current->type = AND;
	else if (buff[pos] == '(')
		return (ft_parse_op_prnts(ast));
	else if (buff[pos] == ')')
		return (ft_parse_cls_prnts(ast));
	ast->current = ast->current->right;
	ret += pos;
	return (ret);
}