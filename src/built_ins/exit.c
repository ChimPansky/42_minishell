/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/08 18:11:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_exit(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void)		fd_out;
	int			exit_code;
	char		*exit_arg;

	ft_printf_err("exit\n");
	exit_arg = cmd_with_args[1];
	if (exit_arg && cmd_with_args[2])
	{
		ft_printf_err("exit: too many arguments\n");
		return (EXIT_FAILURE);
	}
	msh->done = true;
	if (!exit_arg)
		return (msh->last_exit_code);
	exit_code = ft_atol_shift(&exit_arg);
	if (*exit_arg)
		return (ft_printf_err("exit: %s numeric argument required\n", cmd_with_args[1]), EXIT_BUILT_IN_ERROR);
	return (((exit_code % 256) + 256) % 256);
}
