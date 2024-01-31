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
	(void)msh;
	char *cwd;

	if (cmd_with_args[1] != NULL)
		return (ft_printf_err("pwd: too many arguments\n"), EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("pwd"), !EXIT_FAILURE);
	dprintf(fd_out, "%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}
