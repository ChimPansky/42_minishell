/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/28 20:02:34 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_pwd(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void)cmd_with_args;

	update_pwd(msh);
	// if (!msh->pwd)
	// 	return (errno);
	dprintf(fd_out, "%s\n", msh->pwd);
	return (SUCCESS);
}
