/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:39:46 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/09 21:42:24 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/stat.h>

static bool	file_found_on_path(const char *exec_in_path)
{
	struct stat	fstat;

	if (access(exec_in_path, F_OK) != SUCCESS)
		return (false);
	if (SUCCESS != stat(exec_in_path, &fstat))
		return (false);
	return (S_ISREG(fstat.st_mode));
}

static int	try_find_in_path(t_path_finder *pf)
{
	char		**path_entries;
	size_t		i;

	path_entries = ft_split(pf->path, ":");
	if (path_entries == NULL)
		return (perror("ft_split"), !SUCCESS);
	i = 0;
	while (path_entries[i])
	{
		if (ft_str_ends_with(path_entries[i], "/"))
			pf->exec_with_path = ft_strnjoin(2, path_entries[i], pf->exec);
		else
			pf->exec_with_path = ft_strnjoin(3, path_entries[i], "/", pf->exec);
		if (!pf->exec_with_path)
			return (free(path_entries), perror("ft_strnjoin"), !SUCCESS);
		if (file_found_on_path(pf->exec_with_path))
			return (free(path_entries), SUCCESS);
		free(pf->exec_with_path);
		i++;
	}
	pf->exec_with_path = NULL;
	return (free(path_entries), SUCCESS);
}

int	init_path_finder(t_msh *msh, t_path_finder *pf, char *exec_name)
{
	pf->path = varlist_get_value(msh->env, "PATH");
	pf->exec = exec_name;
	pf->exec_with_path = NULL;
	if (NULL == strchr(pf->exec, '/') && *pf->path)
	{
		if (try_find_in_path(pf) != SUCCESS)
			return (!SUCCESS);
	}
	else
	{
		pf->exec_with_path = ft_strdup(pf->exec);
		if (!pf->exec_with_path)
			return (perror("init_path_finder: ft_strdup"), !SUCCESS);
	}
	return (SUCCESS);
}

void	destroy_path_finder(t_path_finder *pf)
{
	free(pf->exec_with_path);
}
