#include "minishell.h"
#include "libft.h"

t_env	*new_env(char *env_line)
{
	t_env *new;
	char **splitted;

	new = (t_env *)malloc(sizeof(t_env));
	splitted = ft_split(env_line, '=');
	new->key = splitted[0];
	new->value = splitted[1];
	new->next = NULL;
	return (new);
}

t_env	*generate_env(char **env)
{
	t_env *head;
	head = NULL;
	while (*env)
	{
		envlist_addback(&head, new_env(*env));
		env++;
	}
	return (head);
}

void print_env_export(t_env *env)
{
    t_env *current;

    current = env;
    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}
