#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include "libft.h"
int has_wildcards(char *str)
{
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}


static int match(char *pattern, char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (1);
	if (*pattern == '*')
	{
		if (match(pattern+1, filename))
			return (1);
		if (*filename && match(pattern, filename+1))
			return (1);
		return (0);
	}
	if (*pattern == *filename)
		return (match(pattern+1, filename+1));
	return (0);
}
static int count_matches(char *pattern)
{
	DIR	*dir;
	struct dirent *entry;
	int count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (!(entry->d_name[0] == '.' && pattern[0] != '.'))
			if (match(pattern, entry->d_name))
				count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}
static char **fill_matches(char *pattern, int count)
{
	DIR *dir;
	struct dirent *entry;
	char **matches;
	int i;
	i = 0;
	matches = malloc(sizeof(char*) * (count+1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		free(matches);
		return (NULL);
	}
	entry = readdir(dir);
	while (entry)
	{
		if (!(entry->d_name[0] == '.' && pattern[0] != '.'))
			if (match(pattern, entry->d_name))
				matches[i++]= ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
	return (matches);
}

static void	sort_string_array(char **arr) // bubble sort for *
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char **expand_wildcards(char *pattern)
{
	int count;
	char **matches;
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	matches = fill_matches(pattern, count);
	if (!matches)
		return (NULL);
	sort_string_array(matches);
	return (matches);
}
