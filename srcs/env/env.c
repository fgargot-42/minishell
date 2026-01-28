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

//key value -> KEY=VALUE char*
static char	*create_env_line(char *key, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value ? value : "");
	free(temp);
	return (result);
}

const char	**reconstruct_envs(t_env *envs)
{
	char **array;
	t_env	*current;

	int count;
	int i;

	//lst count
	count = 0;
	current = envs;
	while (current && ++count)
		current = current->next;

	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);

	i = 0;
	current = envs;

	while (current)
	{
		array[i] = create_env_line(current->key, current->value);
		if (!array[i])
		{
			while (--i >= 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return ((const char**)array);
}
