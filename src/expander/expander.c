/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 18:52:45 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdbool.h>

static void	replace_env(char **input, t_env *env, char *key, size_t *pos)
{
	char	*value;
	char	*new_input;
	size_t	value_end_pos;

	value = "";
	if (env && env->value)
		value = env->value;
	new_input = malloc(sizeof(char) * (ft_strlen(*input) + ft_strlen(value)
				- ft_strlen(key)));
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, *pos + 1);
	ft_strlcpy(&new_input[(*pos)], value, ft_strlen(value) + 1);
	value_end_pos = *pos + ft_strlen(value);
	*pos += ft_strlen(key) + 1;
	ft_strlcpy(&new_input[value_end_pos], &((*input)[*pos]),
		ft_strlen(&((*input)[*pos])) + 1);
	free(*input);
	*input = new_input;
}

static void	replace_errorcode_env(char **input, size_t *pos, t_ctx *ctx)
{
	char	*new_input;
	char	*error_str;
	char	*original;
	size_t	error_len;

	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return ;
	original = *input;
	error_len = ft_strlen(error_str);
	new_input = malloc(sizeof(char) * (ft_strlen(original) + error_len - 1));
	if (!new_input)
	{
		free(error_str);
		return ;
	}
	ft_strlcpy(new_input, original, *pos + 1);
	ft_strlcpy(new_input + *pos, error_str, error_len + 1);
	ft_strlcpy(new_input + *pos + error_len, original + *pos + 2,
		ft_strlen(original + *pos + 2) + 1);
	*pos += error_len;
	free(original);
	free(error_str);
	*input = new_input;
}

static t_env	*get_env(t_list *envs, char *key)
{
	t_list	*node;
	t_env	*env;

	env = NULL;
	node = get_env_node_by_key(envs, key);
	if (node)
		env = (t_env *)node->content;
	return (env);
}

static bool	is_special_dollar(char *input, size_t i, int *open_quotes)
{
	char	next_char;

	if (open_quotes[0])
		return (1);
	next_char = input[i + 1];
	if (open_quotes[1] && (next_char == '\'' || next_char == '\"'))
		return (1);
	return (!ft_isalpha(next_char) && next_char != '_'
		&& next_char != '\'' && next_char != '\"' && next_char != '?');
}

static bool	is_error_code_var(char *input, size_t i)
{
	return (input[i + 1] == '?');
}

static char	*extract_var_name(char *input, size_t start_pos, size_t *end_pos)
{
	size_t	len;
	char	*var_name;

	len = start_pos;
	while (ft_isalnum(input[len]) || input[len] == '_')
		len++;
	*end_pos = len;
	var_name = ft_substr(&input[start_pos], 0, len - start_pos);
	return (var_name);
}

static void	expand_regular_var(char **input, size_t *i, t_list *envs)
{
	char	*var_name;
	t_env	*env;
	size_t	name_start;
	size_t	name_end;

	name_start = *i + 1;
	var_name = extract_var_name(*input, name_start, &name_end);
	if (!var_name)
		return ;
	env = get_env(envs, var_name);
	replace_env(input, env, var_name, i);
	free(var_name);
}

void	expand_var(char **input, t_list *envs, t_ctx *ctx)
{
	size_t	i;
	char	*str;
	int		open_quotes[2];

	i = 0;
	open_quotes[0] = 0;
	open_quotes[1] = 0;
	str = *input;
	while (i < ft_strlen(str))
	{
		if (str[i] == '\'' && !open_quotes[1])
			open_quotes[0] = !open_quotes[0];
		if (str[i] == '\"' && !open_quotes[0])
			open_quotes[1] = !open_quotes[1];
		if (str[i] != '$' || is_special_dollar(str, i, open_quotes))
		{
			i++;
			continue ;
		}
		if (is_error_code_var(str, i))
			replace_errorcode_env(input, &i, ctx);
		else
			expand_regular_var(input, &i, envs);
		str = *input;
		i = 0;
		open_quotes[0] = 0;
		open_quotes[1] = 0;
	}
}

static int	count_words(char *s)
{
	int	res;
	int	open_single;
	int	open_double;

	res = *s != 0;
	open_single = 0;
	open_double = 0;
	while (*s)
	{
		if (*s == '\'' && !open_double)
			open_single = !open_single;
		if (*s == '\"' && !open_single)
			open_double = !open_double;
		if (*s == ' ' && !open_single && !open_double)
			res++;
		s++;
	}
	return (res);
}

static char	*get_next_split_noquote(char *str, int *pos)
{
	int		open_squote;
	int		open_dquote;
	int		start;
	char	*res;

	open_squote = 0;
	open_dquote = 0;
	while (str[*pos] && str[*pos] == ' ')
		(*pos)++;
	start = *pos;
	while (str[*pos])
	{
		open_squote = !open_squote && !open_dquote && str[*pos] == '\'';
		open_dquote = !open_squote && !open_dquote && str[*pos] == '\"';
		if (str[*pos] == ' ' && !open_squote && !open_dquote)
			break ;
		(*pos)++;
	}
	res = malloc(sizeof(char) * (*pos - start + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, &str[start], *pos - start + 1);
	pos++;
	return (res);
}

static char	**ft_split_noquote(char *str)
{
	int		count;
	char	**split;
	int		index;
	int		pos;

	index = 0;
	pos = 0;
	count = count_words(str);
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	split[count] = NULL;
	while (index < count)
	{
		split[index] = get_next_split_noquote(str, &pos);
		if (!split[index])
		{
			free_string_array(split);
			return (NULL);
		}
		index++;
	}
	return (split);
}

static int	split_add(char ***split_str, char *new_string, int pos)
{
	char	**new_split;
	char	**split_res;
	int		split_count;
	int		new_count;
	int		i;

	split_count = 0;
	new_count = 0;
	while ((*split_str)[split_count])
		split_count++;
	new_split = ft_split_noquote(new_string);
	if (!new_split)
		return (0);
	new_count = count_words(new_string);
	split_count += new_count;
	split_res = malloc(sizeof(char *) * split_count);
	if (!split_res)
	{
		free_string_array(new_split);
		return (0);
	}
	i = 0;
	while (i < pos)
	{
		split_res[i] = (*split_str)[i];
		i++;
	}
	while (i < pos + new_count)
	{
		split_res[i] = new_split[i - pos];
		i++;
	}
	free((*split_str)[i - new_count]);
	while (i < split_count)
	{
		split_res[i] = (*split_str)[i - new_count + 1];
		i++;
	}
	free(new_split);
	free(*split_str);
	*split_str = split_res;
	return (new_count);
}

static void	expand_wildcards_in_cmd(t_cmd *cmd)
{
	int		i;
	char	**expanded;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*') != NULL)
		{
			expanded = expand_wildcards(cmd->args[i]);
			if (expanded && expanded[0])
			{
				free(cmd->args[i]);
				cmd->args[i] = ft_strjoin_all_chr(expanded, ' ');
				free_string_array(expanded);
			}
		}
		i++;
	}
}

void	expand_cmd_args(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		i;
	int		new_arg_len;
	char	*tmp;

	i = 0;
	while (node->cmd->args[i])
	{
		expand_var(&node->cmd->args[i], *envs, ctx);
		i++;
	}
	expand_wildcards_in_cmd(node->cmd);
	i = 0;
	while (node->cmd->args[i])
	{
		new_arg_len = 1;
		if (count_words(node->cmd->args[i]) != 1)
			new_arg_len = split_add(&node->cmd->args, node->cmd->args[i], i);
		i += new_arg_len;
	}
	i = 0;
	while (node->cmd->args[i])
	{
		tmp = remove_quotes(node->cmd->args[i]);
		free(node->cmd->args[i]);
		node->cmd->args[i] = tmp;
		i++;
	}
}
