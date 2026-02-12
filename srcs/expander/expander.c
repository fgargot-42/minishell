/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/12 16:57:47 by fgargot          ###   ########.fr       */
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

/*static void	replace_errorcode_env(char **input, size_t *pos, t_ctx *ctx)
{
	char	*new_input;
	char	*error_str;
	size_t	new_pos;
	
	error_str = ft_itoa(ctx->error_code);
	if (!error_str)
		return ;
	new_input = malloc(sizeof(char) * (ft_strlen(*input)
		+ ft_strlen(error_str) - 1));
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, *pos);
	ft_strlcpy(&new_input[(*pos)], error_str, ft_strlen(error_str) + 1);
	new_pos = *pos + ft_strlen(error_str);
	pos += 2;
	ft_strlcpy(&new_input[new_pos], &((*input)[*pos]),
		ft_strlen(((*input)[*pos])) + 1);
	free(*input);
	*input = new_input;
}*/



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

/*void	expand_var(char **input, t_list *envs, t_ctx *ctx)
{
	size_t	len;
	size_t	i;
	t_env	*env;
	char	*key;

	i = 0;
	while (i < ft_strlen(*input))
	{
		if ((*input)[i] == '$')
		{
			if ((*input)[i + 1] == ' ' || (*input)[i+1] == '\0')
				i++;
			else if ((*input)[i + 1] == '?')
				replace_errorcode_env(input, &i, ctx); 
			else
			{
				len = i + 1;
				while (ft_isalnum((*input)[len]) || (*input)[len] == '_')
					len++;
				key = ft_substr(&(*input)[i + 1], 0, len);
				env = get_env(envs, key);
				replace_env(input, env, key, &i);
				free(key);
			}
		}
		i++;
	}
}*/











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

static void	expand_regular_var(t_cmd *cmd, int index, size_t *i, t_list *envs)
{
	char	*var_name;
	t_env	*env;
	size_t	name_start;
	size_t	name_end;
	
	name_start = *i + 1;  // Position after '$'
	var_name = extract_var_name(cmd->args[index], name_start, &name_end);
	if (!var_name)
		return ;
	env = get_env(envs, var_name);
	if (!env && cmd->quote_type[index] == QUOTE_NONE)
	{
		free(cmd->args[index]);
		cmd->args[index] = NULL;
		free(var_name);
		return ;
	}
	replace_env(&cmd->args[index], env, var_name, i);
	free(var_name);
}

void	expand_var(t_cmd *cmd, int index, t_list *envs, t_ctx *ctx)
{
	size_t	i;
	char	*str;
	
	i = 0;
	str = cmd->args[index];
	
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
			replace_errorcode_env(&cmd->args[index], &i, ctx);
		else
			expand_regular_var(cmd, index, &i, envs);
		
		// Update pointer since input may have been reallocated
		str = cmd->args[index];
		if (!str)
		{
			while (cmd->args[index + 1])
			{
				cmd->args[index] = cmd->args[index + 1];
				index++;
			}
			cmd->args[index] = NULL;
			return ;
		}
		i++;
	}
}


static void	expand_regular_var_redir(char **input, size_t *i, t_list *envs)
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

void	expand_var_redir(char **input, t_list *envs, t_ctx *ctx)
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
			expand_regular_var_redir(input, &i, envs);
		
		// Update pointer since input may have been reallocated
		str = *input;
		i++;
	}
}
