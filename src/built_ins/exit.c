/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 20:46:54 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_exit(t_msh *msh, char **cmd_with_args, int fd_out)
{
	char		*exit_arg;

	(void)fd_out;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_printf_err("exit\n");
	msh->done = true;
	if (cmd_with_args[1])
	{
		exit_arg = cmd_with_args[1];
		msh->last_exit_code = ft_atol_shift(&exit_arg);
		if (*exit_arg || !*cmd_with_args[1])
		{
			msh->last_exit_code = BUILT_IN_EXIT_NONUMERIC;
			ft_printf_err("exit: %s numeric argument required\n",
				cmd_with_args[1]);
		}
		else if (cmd_with_args[2])
		{
			ft_printf_err("exit: too many arguments\n");
			msh->done = false;
			msh->last_exit_code = BUILT_IN_EXIT_TOO_MANY_ARGS;
		}
	}
	return (((msh->last_exit_code % 256) + 256) % 256);
}
