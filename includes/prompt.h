/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:47:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/03/18 14:12:10 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_prompt_parts
{
	char	*exit_code;
	char	*color;
	char	*icon;
	char	*name;
	char	*cwd;
	char	*sep;
}			t_prompt_parts;

char	*build_prompt(int exit_code);
