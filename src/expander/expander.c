/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/02/19 23:25:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

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
		check_open_quotes(str[i], open_quotes);
		if (str[i] != '$' || is_special_dollar(str, i, open_quotes))
		{
			i++;
			continue ;
		}
		if (str[i + 1] == '?')
			replace_errorcode_env(input, &i, ctx);
		else
			expand_regular_var(input, &i, envs);
		str = *input;
		i = 0;
		open_quotes[0] = 0;
		open_quotes[1] = 0;
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
