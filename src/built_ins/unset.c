/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 21:55:15 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_unset(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void) fd_out;
	while (*(++cmd_with_args))
	{
		if (**cmd_with_args == '-' && *(*cmd_with_args + 1))
			return (ft_printf_err("msh: unset: unsupported option -%c\n",
					*(*cmd_with_args + 1)), EXIT_FAILURE);
		varlist_delete_one(&msh->env, *cmd_with_args);
	}
	return (EXIT_SUCCESS);
}
