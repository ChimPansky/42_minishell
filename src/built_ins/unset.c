/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 15:29:55 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_unset(t_msh *msh, char **cmd_with_args, int out_fd)
{
	int	i;

	i = 1;
	// problem with unsetting several variables at the same time...
	while (cmd_with_args[i])
	{
		if (1)	// validate var_name...
			var_delete(&msh->env, cmd_with_args[i]);
		else
			dprintf(out_fd, "unset: invalid variable name"); // stderr or out? error handling...
		i++;
	}
	return (SUCCESS);
}
