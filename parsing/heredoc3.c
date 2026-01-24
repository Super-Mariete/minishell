/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 11:02:33 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/18 11:02:33 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	key_len(const char *line)
{
	size_t	len;

	len = 0;
	while (!ft_strchr(NO_VAL_VAR, line[len]))
		len++;
	return (len);
}

static char	*get_var(char *key, const t_shenv *env)
{
	size_t	len;

	len = key_len(key);
	while (env)
	{
		if (!ft_strncmp(key, env->var, len) && *(env->var + len) == '=')
			return (env->var + len + 1);
		env = env->next;
	}
	return (NULL);
}

static char	*copy_to_buffer(char *line, char *buffer, const t_shenv *env)
{
	size_t	i;
	size_t	j;
	char	*var;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			var = get_var(line + i + 1, env);
			if (var)
				ft_strcpy(buffer + j, var);
			j += ft_strlen(var);
			i += key_len(line + i + 1) + 1;
			continue ;
		}
		buffer[j] = line[i];
		i++;
		j++;
	}
	return (buffer);
}

static char	*expanded_line(char *line, const t_shenv *env)
{
	size_t		i;
	size_t		len;
	const char	*var;
	char		*expanded;

	i = 0;
	len = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			var = get_var(line + i + 1, env);
			len += ft_strlen(var);
			i += key_len(line + i + 1) + 1;
		}
		i++;
		len++;
	}
	expanded = (char *)ft_calloc(len + 1, sizeof(char));
	if (!expanded)
		return (perror("minishell: malloc:"), NULL);
	copy_to_buffer(line, expanded, env);
	return (expanded);
}

char	**expand_array(char **array, const t_cli *cli)
{
	size_t	i;
	char	*expanded;

	i = 0;
	while (array[i])
	{
		if (ft_strchr(array[i], '$'))
		{
			expanded = expanded_line(array[i], *(cli->env));
			if (!expanded && errno == ENOMEM)
				return (ft_free_d(array), NULL);
			if (!expanded)
				expanded = ft_strdup("\0");
			if (!expanded)
				return (ft_free_d(array), NULL);
			free(array[i]);
			array[i] = expanded;
		}
		i++;
	}
	return (array);
}
