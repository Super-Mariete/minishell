/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:55 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/25 20:33:56 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_redirs(char **token, int i)
{
	char	*s;

	if (token[i] && ft_strchr(REDIR_S, token[i][0]) && (!token[i][1] || (token[i][1] == token[i][0] && !token[i][2])))
	{
		if (!token[i + 1])
			return (ft_perror_token("newline", SYN_ERR), 1);
		s = ft_strndup(token[i], 3);
		ft_perror_token(s, SYN_ERR);
		free(s);
		return (1);
	}
	return (0);
}

int	ft_check_errors(char **token, int len)
{
	int	i;

	if (!token)
		return (1);
	if (token[0] && ft_strchr(OP_STR2, token[0][0]))
		return (printf("1\n"), ft_perror_token(token[0], SYN_ERR), 1);
	i = 0;
	while (i < len)
	{
		// printf("token[%d] = %s\n", i, token[i]);
		if (token[i] && ft_strchr(OP_STR2, token[i][0]) && (token[i + 1] && ft_strchr(OP_STR2, token[i + 1][0])))
			return (printf("2\n"), ft_perror_token(token[i + 1], SYN_ERR), 1);
		else if (token[i] && token[i][0] == ')' && (token[i + 1] && !ft_strchr(OP_STR, token[i + 1][0])))
			return (printf("3\n"), ft_perror_token(token[i + 1], SYN_ERR), 1);
		else if (token[i] && token[i][0] == '(' && i > 0 && (!ft_strchr(OP_STR, token[i - 1][0])))
			return (printf("4\n"), ft_perror_token(token[i + 1], SYN_ERR), 1);
		else if (token[i] && token[i][0] == '(' && token[i + 1] && token[i + 1][0] == ')')
			return (printf("5\n"), ft_perror_token(token[i + 1], SYN_ERR), 1);
		else if (token[i] && ft_strchr(OP_STR, token[i][0]) && !token[i + 1])
			return (printf("6\n"), ft_perror_token(token[i], SYN_ERR), 1);
		else if (token[i] && ft_strchr(SEP_STR, token[i][0]) && token[i + 1] && ft_strchr(SEP_STR, token[i + 1][0]))
			return (printf("7\n"), ft_perror_token(token[i + 1], SYN_ERR), 1);
		i++;
	}
	return (0);
}

int	ft_sep_len(char *line, int pos)
{
	char	*err;
	char	*t;
	int		i;

	if (!line || pos < 0)
		return (-1);
	if (ft_isspace(line[pos]))
		return (0);
	if (ft_strchr(SEP_STR, line[pos]) && line[pos + 1] == line[pos])
		return (2);
	if (ft_strchr(SEP_STR, line[pos]))
		return (1);
	return (1);
}

int	ft_token_len(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	// printf("line = '%s'\n", line);
	while (i < len && ft_isspace(line[i]))
		i++;
	// printf("line2 = '%s'\n", line + i);
	if (ft_strchr(SEP_STR, line[i]))
		return (/*printf("t_len = %d\n", i + ft_sep_len(line, i)), */i + ft_sep_len(line, i));
	// printf("i = %d\n", i);
	while (i < len)
	{
		if (ft_strchr(QUOTES, line[i]) && (i == 0 || (i > 0 && line[i - 1] != '\\')))
		{
			if (ft_quoted_len(line + i, line[i]) <= 0)
				return (-1);
			i = (ft_quoted_len(line + i, line[i]) + i);
			continue ;
		}
		if (ft_strchr(SEP_STR, line[i]))
			return (/*printf("t_len2 = %d\n", i), */i);			
		i++;
	}
	return (i);
}

int	ft_num_s_tokens(char *line)
{
	int	i;
	int	len;
	int	line_len;
	int	num_token;

	if (!line)
		return (0);
	i = 0;
	num_token = 0;
	line_len = ft_strlen(line);
	while (i < line_len)
	{
		if (ft_isspace(line[i]) && (ft_isspace(line[i + 1]) || !line[i + 1]))
		{
			i++;
			continue ;
		}
		len = ft_token_len(line + i);
		if (len <= 0)
			return (-1);
		i += len;
		num_token++;
	}
	return (num_token);
}

char	**ft_token_sep(char *line)
{
	int		i;
	int		j;
	int		len;
	char	**tokens;

	if (!line)
		return (NULL);
	len = ft_num_s_tokens(line);
	// printf("n_token = %d\n", len);
	if (len <= 0)
		return (free(line), NULL);
	tokens = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!tokens)
		return (free(line), perror("minishell: malloc : "), NULL);
	i = 0;
	j = 0;
	while (tokens && i < ft_strlen(line))
	{
		while (ft_isspace(line[i]))
			i++;
		if (ft_token_len(line + i) <= 0)
			return (tokens[len] = NULL, ft_free_d(tokens), free(line), NULL);
		tokens[j++] = ft_strndup(line + i, ft_token_len(line + i));
		// printf("tokens[%d] = %s\n", j - 1, tokens[j - 1]);
		i += ft_token_len(line + i);
	}
	return (free(line), tokens[len] = NULL, tokens);
}
