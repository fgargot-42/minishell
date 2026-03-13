/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/13 23:02:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static size_t	expand_regular_var(char **input, size_t i, t_list *envs)
{
	char	*var_name;
	t_env	*env;
	size_t	name_start;
	size_t	name_end;
	size_t	new_index;

	name_start = i + 1;
	var_name = extract_var_name(*input, name_start, &name_end);
	if (!var_name)
		return (i + 1);
	env = get_env(envs, var_name);
	new_index = replace_regular_env(input, env, var_name, i);
	free(var_name);
	return (new_index);
}

void	expand_var(char **input, t_list *envs, t_ctx *ctx)
{
	size_t	i;
	int		open_quotes[2];

	i = 0;
	open_quotes[0] = 0;
	open_quotes[1] = 0;
	if (!input || !(*input))
		return ;
	while (i < ft_strlen((*input)))
	{
		update_open_quotes((*input)[i], open_quotes);
		if ((*input)[i] != '$' || is_special_dollar(*input, i, open_quotes))
			i++;
		else if ((*input)[i + 1] == '?')
			i = replace_errorcode_env(input, i, ctx);
		else if (ft_isdigit((*input)[i + 1]))
			i = replace_numeric_env(input, i, ctx);
		else
			i = expand_regular_var(input, i, envs);
	}
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
			if (expanded)
			{
				if (expanded[0])
				{
					free(cmd->args[i]);
					cmd->args[i] = ft_strjoin_all_chr(expanded, ' ');
					free_string_array(expanded);
				}
				else
					free(expanded);
			}
		}
		i++;
	}
}

static void	split_expanded_args(t_node *node)
{
	int	i;
	int	new_arg_len;

	i = 0;
	while (node->cmd->args[i])
	{
		new_arg_len = 1;
		if (count_words(node->cmd->args[i]) != 1)
			new_arg_len = split_add(&node->cmd->args, node->cmd->args[i], i);
		i += new_arg_len;
	}
}

void	expand_cmd_args(t_node *node, t_list **envs, t_ctx *ctx)
{
	int		i;
	char	*tmp;

	i = 0;
	while (node->cmd->args[i])
	{
		expand_var(&node->cmd->args[i], *envs, ctx);
		i++;
	}
	expand_wildcards_in_cmd(node->cmd);
	split_expanded_args(node);
	i = 0;
	while (node->cmd->args[i])
	{
		tmp = remove_quotes(node->cmd->args[i]);
		free(node->cmd->args[i]);
		node->cmd->args[i] = tmp;
		i++;
	}
}
