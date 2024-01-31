/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/27 22:15:04 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_exit(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void) fd_out;
	ft_printf_err("exit\n");
	if (cmd_with_args[2])
	{
		ft_printf_err("too many args\n");
		errno = 1;
		return (errno);
	}
	if (cmd_with_args[1])
	{
		if (1) // check if argument is numeric and not too large (> long??)
			ms_exit(msh, ft_atoi(cmd_with_args[1]) % 256);
		else
		{
			ft_printf_err("numeric argument required\n");
			errno = 2;
			ms_exit(msh, SUCCESS);
		}
	}
	errno = msh->last_exit_code;
	ms_exit(msh, SUCCESS);
	return (SUCCESS);
}
