/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/16 19:16:53 by mabarrer         ###   ########.fr       */
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
	size_t	before_len;
	size_t	error_len;
	size_t	after_start;
	
	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return ;
	original = *input;
	before_len = *pos;           // Length before "$?"
	error_len = ft_strlen(error_str);
	after_start = *pos + 2;      // Position after "$?" (skip 2 chars)
	new_input = malloc(sizeof(char) * (ft_strlen(original) + error_len - 1));
	if (!new_input)
	{
		free(error_str);
		return ;
	}
	ft_strlcpy(new_input, original, before_len + 1);
	ft_strlcpy(new_input + before_len, error_str, error_len + 1);
	ft_strlcpy(new_input + before_len + error_len,
		original + after_start,
		ft_strlen(original + after_start) + 1);
	*pos = before_len + error_len;
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
static bool	is_special_dollar(char *input, size_t i)
{
	char next_char;
	
	next_char = input[i + 1];
	return (next_char == ' ' || next_char == '\0');
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
	
	name_start = *i + 1;  // Position after '$'
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
	
	i = 0;
	str = *input;
	
	while (i < ft_strlen(str))
	{
		if (str[i] != '$')
		{
			i++;
			continue ;
		}
		
		// Handle different types of $ expansion
		if (is_special_dollar(str, i))
			i++;
		else if (is_error_code_var(str, i))
			replace_errorcode_env(input, &i, ctx);
		else
			expand_regular_var(input, &i, envs);
		
		// Update pointer since input may have been reallocated
		str = *input;
		i++;
	}
}

static int	count_spaces(char *s)
{
	int	res;
	int	open_single;
	int	open_double;

	res = 0;
	open_single = 0;
	open_double = 0;
	while (*s)
	{
		if (*s == '\'' && !open_double)
			open_single = !open_single;
		if (*s == '\"' && !open_single)
			open_double = !open_double;
		if (*s ==  ' ' && !open_single && !open_double)
			res++;
		s++;
	}
	return (res);
}

static char *get_next_split_noquote(char *str)
{
	static int	pos = 0;
	int			open_squote;
	int			open_dquote;
	int			start;
	char		*res;
	
	open_squote = 0;
	open_dquote = 0;
	while (str[pos] &&  str[pos] == ' ')
		pos++;
	start = pos;
	while (str[pos])
	{
		open_squote = !open_squote && !open_dquote && str[pos] == '\'';
		open_dquote = !open_squote && !open_dquote && str[pos] == '\"';
		if (str[pos] == ' ' && !open_squote && !open_dquote)
			break ;
		pos++;
	}
	res = malloc(sizeof(char) * (pos - start + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, &str[start], pos - start + 1);
	pos++;
	return (res);
}

static char	**ft_split_noquote(char *str)
{
	int		count;
	char	**split;
	int		index;

	index = 0;
	count = count_spaces(str) + 1;
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	split[count] = NULL;
	while (index < count)
	{
		split[index] = get_next_split_noquote(str);
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
	new_count =  0;
	while (split_str[split_count])
		split_count++;
	new_split = ft_split_noquote(new_string);
	if (!new_split)
		return (0);
	new_count = count_spaces(new_string) + 1;
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
	while (i < split_count - 1)
	{
		split_res[i] = (*split_str)[i - new_count + 1];
		i++;
	}
	split_res[i] = NULL;
	free(*split_str);
	*split_str = split_res;
	return (new_count);
}

static void	quote_type_add(t_quote_type **quotes, int pos, int count)
{
	int				initial_count;
	int				i;
	t_quote_type	*new_quotes;

	initial_count = 0;
	while (quotes[initial_count])
		initial_count++;
	new_quotes = malloc(sizeof(t_quote_type) * (count + initial_count));
	if (!new_quotes)
		return ;
	i = 0;
	while (i < initial_count + count)
	{
		if (i < pos)
			new_quotes[i] = (*quotes)[i];
		else if (i < pos + count)
			new_quotes[i] = new_quotes[i - 1];
		else
			new_quotes[i] = (*quotes)[i - count];
		i++;
	}
	free(*quotes);
	*quotes = new_quotes;
}

static void expand_wildcards_simple(t_cmd *cmd)
{
	int		i;
	char	**expanded;

	if (!cmd || !cmd->args)
		return;
	i = 0;
	while (cmd->args[i])
	{
		if (cmd->quote_type[i] == QUOTE_NONE && has_wildcards(cmd->args[i]))
		{
			expanded = expand_wildcards(cmd->args[i]);
			if (expanded && expanded[0])
			{
				free(cmd->args[i]);
				cmd->args[i] = ft_strdup(expanded[0]);
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

	i = 0;
	while (node->cmd->args[i])
	{
		if (node->cmd->quote_type[i] != QUOTE_SINGLE)
			expand_var(&node->cmd->args[i], *envs, ctx);
		i++;
	}
	i = 0;
	while (node->cmd->args[i])
	{
		new_arg_len = 0;
		if (node->cmd->quote_type[i] == QUOTE_NONE && count_spaces(node->cmd->args[i]))
		{
			new_arg_len = split_add(&node->cmd->args, node->cmd->args[i], i);
			quote_type_add(&node->cmd->quote_type, i, new_arg_len);
		}
		i += new_arg_len + 1;
	}
	expand_wildcards_simple(node->cmd);
}


