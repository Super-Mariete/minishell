/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 19:40:47 by rafael            #+#    #+#             */
/*   Updated: 2026/01/05 18:34:37 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	ft_parse_op(char *buff, size_t pos, t_ast *ast)
{
	size_t	ret;
	size_t	next;
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
	{
		ast->current->type = OPEN_PRTS;
		ret = 1;
	}
	ast->current->left = &ast->first[ast->n_nodes + 1];
	next = ft_next_token(buff, pos);
	if (ft_strchr(CONTROL_OP, buff[next]))
	{
		ft_perror_str_token(&(buff[next]), UNEXPTKN);
		return (SIZE_MAX);
	}
	ret += pos;
	return (ret);
}

size_t	ft_append(char *buff, size_t pos, t_ast *ast)
{
	size_t	i;
	size_t	next;

	i = ft_next_token(buff, pos);
	if (!buff[i])
	{
		ft_perror_str_token(">>", NOARGS);
		return (SIZE_MAX);
	}
	next = ft_next_token(buff, i);
	if (ft_strchr(CONTROL_OP, buff[next]))
	{
		ft_perror_str_token(&buff[i], UNEXPTKN);
		return (SIZE_MAX);
	}
	ast->current->append = 1;
	ast->current->outfile = buff + i;
	if (next == i)
		return (SIZE_MAX);
	return (i);
}

// char	*ft_cmd_path(char *buffer, t_env *env)
// {
	
// }

// static int	ft_parse_cmd(char *buff, size_t pos, t_ast *ast, t_read *read)
// {
// 	if (!ft_strcmp(&(buff[pos]), "echo")
// 	|| !ft_strcmp(&(buff[pos]), "cd") || !ft_strcmp(&(buff[pos]), "pwd")
// 	|| !ft_strcmp(&(buff[pos]), "export") || !ft_strcmp(&(buff[pos]), "unset")
// 	|| !ft_strcmp(&(buff[pos]), "env") || !ft_strcmp(&(buff[pos]), "exit"))
// 	{
// 		ast->current->is_builtin = 1;
// 		ast->current->cmd = &(buff[pos]);
// 		return (1);
// 	}
// 	if (ft_strchr(&(buff[pos]), '/'))
// 		ast->current->cmd = &(buff[pos]);
	
// 	// else
// 	// 	cli->cmd = ft_cmd_path(getenv("PATH"), &(buff[pos]));
// 	// if (!cli->cmd)
// 	// 	cli->cmd = ft_strdup(&(buff[pos]));
// 	return (1);
// }

size_t	ft_parse(t_msh *msh)
{
	size_t	i;
	size_t	len;
	char	*buff;
	t_ast	*ast;

	if (ft_lexer(msh))
		return (*msh->status);
	i = 0;
	ast = msh->ast;
	len = msh->buff->last - msh->buff->buffer;
	buff = msh->buff->buffer;
	while (i < len)
	{
		if (!ft_strncmp(buff + i, ">>", 2))
			i = ft_append(buff + i, i, ast);
		else if (ft_strchr(OP, buff[i]))
			i = ft_parse_op(buff, i, msh->ast);
		// else if (!ast->current->cmd)
		// 	ft_parse_cmd();
		// else if (!ft_strncmp(buff[i], "<<", 2))
		// {
		// 	if (ft_heredoc(buff[++i], buff) == 130)
		// 		return (130);

		// else
		// 	i = ft_parse_token(buff, i, cli, &group);
		if (i == SIZE_MAX)
		{
			*msh->status = 2;
			return (2);
		}
		break ;
	}
	// ft_print_ast(msh->ast);
	ft_reset_buffer(msh->buff);
	return (i);
}