/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 19:40:47 by rafael            #+#    #+#             */
/*   Updated: 2026/01/07 11:36:32 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_append(char *buff, size_t pos, t_msh *msh)
{
	size_t	i;
	t_ast	*ast;

	ast = msh->ast;
	i = pos + 3;
	if (!buff[i])
	{
		ft_perror_str_token("'newline'", UNEXPTKN);
		return (SIZE_MAX - 1);
	}
	if (ft_strchr(CONTROL_OP, buff[i]))
	{
		ft_perror_str_token(&buff[i], UNEXPTKN);
		return (SIZE_MAX - 1);
	}
	if (ft_assign_cmd(ast->current, msh))
		return (SIZE_MAX - 1);
	ast->current->cmd->append = 1;
	ast->current->cmd->outfile = buff + i;
	i += ft_strlen(&(buff[i])) ;
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

// static void ft_print_buff(const char *s, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i <= len)
// 	{
// 		printf("i = %zu, %c\n", i, s[i]);
// 		i++;
// 	}
// 	write(1, "\n", 1);
// }

size_t	ft_parse(t_msh *msh)
{
	size_t			i;
	size_t			len;
	char			*buff;
	
	len = ft_lexer(msh);
	if (len) 
		return (*msh->status);
	i = 0;
	len = msh->buff->last - 1 - msh->buff->buffer;
	// printf("len = %zu\n", len);
	buff = msh->buff->buffer;
	// ft_print_buff(buff, len);
	while (i < len)
	{
		// printf("token = %s, i = %zu\n", &(buff[i]), i);
		if (buff[i] && !ft_strncmp(buff + i, ">>", 2))
			i = ft_append(buff, i, msh);
		else if (buff[i] && ft_strchr(OP, buff[i]))
			i = ft_parse_op(buff, i, msh->ast);
		// else if (!ast->current->cmd)
		// 	ft_parse_cmd();
		// else if (!ft_strncmp(buff[i], "<<", 2))
		// {
		// 	if (ft_heredoc(buff[++i], buff) == 130)
		// 		return (130);
		// else
		// 	i = ft_parse_token(buff, i, cli, &group);
		// printf("i = %zu\n", i);
		if (i == SIZE_MAX - 1)
			*msh->status = 2;
		i++;
		// break ;
	}
	ft_print_ast(msh->ast);
	ft_reset_read(msh->rbuffer);
	ft_reset_buffer(msh->buff);
	return (i);
}
