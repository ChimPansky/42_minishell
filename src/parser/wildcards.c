#include "libft.h"
#include "ft_charptr_array.h"
#include "parser.h"
#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>

static bool	does_string_match_wildcard_expression(
		char *string, char *pattern, t_list *true_wcs)
{
	size_t	len;
	size_t	idx;

	idx = 0;
	len = *((size_t *)true_wcs->content);
	if (!ft_str_start_with(string, pattern, len))
		return (false);
	string += len;
	idx += len + 1;
	true_wcs = true_wcs->next;
	while (true_wcs)
	{
		len = *((size_t *)true_wcs->content) - idx;
		string = ft_strstrn(string, pattern + idx, len);
		if (string == NULL)
			return (false);
		string += len;
		idx += len + 1;
		true_wcs = true_wcs->next;
	}
	return (ft_str_end_with(string, pattern + idx));
}

static int	check_dir(char *name, t_expander *expander, t_charptr_array *arr)
{
	if (ft_strcmp(name, ".") == SUCCESS || ft_strcmp(name, "..") == SUCCESS)
		return (SUCCESS);
	if (name[0] == '.' && expander->replace.buf[0] != '.')
		return (SUCCESS);
	if (!does_string_match_wildcard_expression(
			name, expander->replace.buf, expander->true_wildcards))
		return (SUCCESS);
	if (charptr_array_dup_and_add_str(arr, name) != SUCCESS)
		return (perror("check_dir: charptr_array_dup_and_add_str"),
			!SUCCESS);
	return (SUCCESS);
}

// take pattern by ptrptr and free if added
int	expand_wildcard_and_finalize(t_expander *expander, t_charptr_array *arr)
{
	DIR				*cur_dir;
	struct dirent	*dir_descr;
	const size_t	start_sz = arr->sz;

	cur_dir = NULL;
	if (expander->true_wildcards != NULL)
		cur_dir = opendir(".");
	if (cur_dir)
	{
		dir_descr = readdir(cur_dir);
		while (dir_descr != NULL)
		{
			if (SUCCESS != check_dir(dir_descr->d_name, expander, arr))
				return (closedir(cur_dir), !SUCCESS);
			dir_descr = readdir(cur_dir);
		}
	}
	if (cur_dir)
		closedir(cur_dir);
	if (start_sz != arr->sz)
		return (SUCCESS);
	if (charptr_array_add_allocated_str(arr, &expander->replace.buf) != SUCCESS)
		return (perror("expand_wildcard: charptr_array_add_allocated_str"),
			!SUCCESS);
	return (SUCCESS);
}
