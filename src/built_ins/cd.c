/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 11:00:04 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_cd(t_msh *msh, char **cmd_with_args)
{
	int	return_code;

	return_code = 0;
	if (!cmd_with_args[1])	// no args - do nothing
		return (SUCCESS);
	if (cmd_with_args[2])	// too many args
		return (1);
	return_code = chdir(cmd_with_args[1]);
	if (return_code != 0)	//error_handling...
		printf("cd: error\n");
	else
	{
		update_pwd(msh);
		update_prompt(msh);
	}
	return (return_code);
}
