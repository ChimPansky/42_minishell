/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:48:14 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/08 19:48:15 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_charptr_array.h"
#include <sys/stat.h>

// fileno in child process always STDOUT_FILENO
static int	try_exec_built_in(t_msh *msh, char **cmd_with_args)
{
	const t_built_in	func = get_built_in_by_name(cmd_with_args[0]);

	if (func == NULL)
		return (!SUCCESS);
	msh->last_exit_code = func(msh, cmd_with_args, STDOUT_FILENO);
	return (SUCCESS);
}

static int	check_permissions(t_msh *msh, const char *exec)
{
	struct stat	fstat;

	if (exec == NULL || SUCCESS != access(exec, F_OK))
		msh->last_exit_code = EXIT_COMMAND_NOT_FOUND;
	if (exec == NULL)
		return (ft_printf_err("msh: command not found: %s\n", exec), !SUCCESS);
	if (SUCCESS != access(exec, F_OK))
		return (ft_printf_err("msh: %s: no such file or directory\n", exec),
			!SUCCESS);
	if (SUCCESS != stat(exec, &fstat))
		return (perror("check_permissions: stat"), !SUCCESS);
	if (!S_ISDIR(fstat.st_mode) || SUCCESS != access(exec, X_OK))
		msh->last_exit_code = EXIT_PERMISSION_DENIED;
	if (!S_ISDIR(fstat.st_mode))
		return (ft_printf_err("msh: %s: is a directory\n", exec), !SUCCESS);
	if (SUCCESS != access(exec, X_OK))
		return (ft_printf_err("msh: %s: permission denied\n", exec), !SUCCESS);
	return (SUCCESS);
}

// exit code is set to EXIT_FAILURE higher on stack
// return value may be skipped
int	execute_in_child_process(t_msh *msh, char **cmd_with_args)
{
	t_charptr_array		envp;
	t_path_finder		pf;

	if (NULL == strchr(cmd_with_args[0], '/')
		&& try_exec_built_in(msh, cmd_with_args) == SUCCESS)
		return (SUCCESS);
	if (SUCCESS != init_path_finder(msh, &pf, cmd_with_args[0]))
		return (!SUCCESS);
	if (check_permissions(msh, pf.exec) != SUCCESS)
		return (destroy_path_finder(&pf), SUCCESS);
	if (varlist_convert_to_array(msh->env, &envp) != SUCCESS)
		return (destroy_path_finder(&pf), perror("execute_in_child_process: "
				"varlist_convert_to_array"), !SUCCESS);
	execve(pf.exec_with_path, cmd_with_args, envp.buf);
	return (perror("execute_in_child_process: exeve"),
		charptr_array_destroy(&envp), destroy_path_finder(&pf), !SUCCESS);
}
