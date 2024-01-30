/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/09 19:33:08 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <sys/stat.h>

int check_permissions(const char* dir)
{
    struct stat fstat;

	if (SUCCESS != access(dir, F_OK))
		return (ft_dprintf(STDERR_FILENO,
				"cd: %s: no such file or directory\n", dir), !SUCCESS);
    if (SUCCESS != stat(dir, &fstat))
		return (ft_dprintf(STDERR_FILENO,
				"cd: %s: ", dir), perror("stat"), !SUCCESS);
    if (!S_ISDIR(fstat.st_mode))
		return (ft_dprintf(STDERR_FILENO,
				"cd: %s: not a directory\n", dir), !SUCCESS);
	if (SUCCESS != access(dir, X_OK))
		return (ft_dprintf(STDERR_FILENO,
				"cd: %s: permission denied\n", dir), !SUCCESS);
	return (SUCCESS);
}

int	built_in_cd(t_msh *msh, char **cmd_with_args, int fd_out)
{
	char	*dir;

	if (cmd_with_args[2])
		return(ft_dprintf(STDERR_FILENO, "cd: too many arguments"), EXIT_FAILURE);
	if (!cmd_with_args[1])
		dir = var_get_value(msh->env, "HOME");
	else if (strcmp(cmd_with_args[1], "-") == SUCCESS)
	{
		dir = var_get_value(msh->env, "OLDPWD");
		ft_dprintf(fd_out, "%s\n", dir);
	}
	else
		dir = cmd_with_args[1];
	if (dir[0] == '\0')
		return EXIT_SUCCESS;
	if (check_permissions(dir) != SUCCESS)
		return EXIT_FAILURE;
	if (chdir(dir) != SUCCESS)
		return (ft_dprintf(STDERR_FILENO,
				"cd: %s: ", dir), perror("chdir"), EXIT_FAILURE);
	var_set(&msh->env, "OLDPWD", var_get_value(msh->env, "PWD"));
	dir = getcwd(NULL, 0);
	var_set(&msh->env, "PWD", dir);
	free(dir);
	return (EXIT_SUCCESS);
}
