#include "minishell.h"
#include "string.h"
int	is_builtin(t_cmd *cmd)
{
	int	i;
	const char *c = cmd->args[0];
	const char *cmds[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"
	};
	i = 0;
	while (i < 7)
	{
		if (strcmp(c, cmds[i++]) == 0)
			return (1);
	}
	return (0);
}
int	call_builtin(t_cmd *cmd, t_env *envs)
{
	char *c;
	int i;
	c = cmd->args[0];

	const char *cmds[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"
	};

	t_builtin_func cmds_func[] = {
		builtin_echo,
		builtin_cd,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
		builtin_exit
	};
	i = 0;
	while (i < 7)
	{
		if (strcmp(c, cmds[i]) == 0)
			return (cmds_func[i](cmd, envs));
		i++;
	}
	return (43);

}
