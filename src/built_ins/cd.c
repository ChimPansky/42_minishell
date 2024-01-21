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

// TODO '~', '-'
int	built_in_cd(t_msh *msh, char **cmd_with_args, int out_fd)
{
	int		return_code;
	char	*dir;

	return_code = 0;
	dir = NULL;
	if (cmd_with_args[2]) // too many args; error handling...
		return (1);
	if (!cmd_with_args[1])
	{
		dir = var_get_value(msh->env, "HOME");
		if (!dir)
			dir = "";
	}
	else
		dir = cmd_with_args[1]; // implement cd - ?
	return_code = chdir(dir);
	if (return_code != 0)	//error_handling...
		dprintf(out_fd, "cd: error\n");
	else
	{
		// var_set OLD_PWD...
		update_pwd(msh);
		update_prompt(msh);
	}
	return (return_code);
}
