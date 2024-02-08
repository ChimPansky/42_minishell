/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/08 17:31:06 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_pwd(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void)msh;
	(void)cmd_with_args;
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("pwd"), !EXIT_FAILURE);
	ft_dprintf(fd_out, "%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}
