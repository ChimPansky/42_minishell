/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 18:10:51 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_cd(t_msh *msh, char **cmd_with_args)
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
		printf("cd: error\n");
	else
	{
		// var_set OLD_PWD...
		update_pwd(msh);
		update_prompt(msh);
	}
	return (return_code);
}
