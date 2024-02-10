/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 21:20:53 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <sys/stat.h>

int	check_permissions(const char *dir)
{
	struct stat	fstat;

	if (SUCCESS != access(dir, F_OK))
		return (ft_printf_err("cd: %s: no such file or directory\n", dir),
			!SUCCESS);
	if (SUCCESS != stat(dir, &fstat))
		return (ft_printf_err("cd: %s: \n", dir), perror("stat"), !SUCCESS);
	if (!S_ISDIR(fstat.st_mode))
		return (ft_printf_err("cd: %s: not a directory\n", dir), !SUCCESS);
	if (SUCCESS != access(dir, X_OK))
		return (ft_printf_err("cd: %s: permission denied\n", dir), !SUCCESS);
	return (SUCCESS);
}

int	built_in_cd(t_msh *msh, char **cmd_with_args, int fd_out)
{
	char	*dir;

	if (cmd_with_args[1] && cmd_with_args[2])
		return (ft_printf_err("cd: too many arguments\n"), EXIT_FAILURE);
	if (!cmd_with_args[1])
		dir = varlist_get_value(msh->env, "HOME");
	else if (ft_strcmp(cmd_with_args[1], "-") == SUCCESS)
		dir = varlist_get_value(msh->env, "OLDPWD");
	else
		dir = cmd_with_args[1];
	if (dir[0] == '\0')
		dir = ".";
	if (check_permissions(dir) != SUCCESS)
		return (EXIT_FAILURE);
	if (chdir(dir) != SUCCESS)
		return (ft_printf_err("cd: %s: ", dir), perror("chdir"), EXIT_FAILURE);
	if (cmd_with_args[1] && ft_strcmp(cmd_with_args[1], "-") == SUCCESS)
		ft_dprintf(fd_out, "%s\n", dir);
	varlist_set(&msh->env, "OLDPWD", varlist_get_value(msh->env, "PWD"));
	dir = getcwd(NULL, 0);
	varlist_set(&msh->env, "PWD", dir);
	free(dir);
	return (EXIT_SUCCESS);
}
