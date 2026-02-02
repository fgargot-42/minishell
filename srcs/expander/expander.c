#include "libft.h"
#include "minishell.h"

char	*expand_var(char *input, t_list *envs, t_ctx *ctx)
{
	char	*res;
	size_t	len;
	size_t	i;
	char	*key;
	char	*value;

	res = malloc(100000);
	i = 0;
	while (*input)
	{
		if (*input == '$')
		{
			input++;
			if (*input == '?')
			{
				input++;
				const char *strerrcode = ft_itoa(ctx->error_code);
				strcat(res, strerrcode);
				i += strlen(strerrcode);
			}
			else
			{
				len = 0;
				while (ft_isalnum(*input) || *input == '_')
				{
					input++;
					len++;
				}
				key = ft_substr(input - len, 0, len);
				value = ((t_env *)get_env_node_by_key(envs, key)->content)->value;
				strcat(res, value);
				i += strlen(value);
			}
		}
		else
		{
			res[i++] = *input;
			input++;
		}
	}
	res[i] = 0;
	return (res);
}
