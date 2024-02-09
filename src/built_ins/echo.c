/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 21:54:40 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "libft.h"
#include <stdlib.h>

bool	is_n_option(char *arg)
{
	if (*arg != '-' || arg[1] != 'n')
		return (false);
	while (*++arg)
	{
		if (*arg != 'n')
			return (false);
	}
	return (true);
}

static int	handle_echo(char **cmd_with_args, int fd_out)
{
	bool	reading_options;
	bool	has_n_option;

	reading_options = true;
	has_n_option = false;
	while (*cmd_with_args)
	{
		if (reading_options)
		{
			if (is_n_option(*cmd_with_args))
				has_n_option = true;
			else
				reading_options = false;
		}
		if (!reading_options)
		{
			if (ft_dprintf(fd_out, "%s", *cmd_with_args) == -1)
				return (!SUCCESS);
			if (cmd_with_args[1])
				ft_dprintf(fd_out, " ");
		}
		cmd_with_args++;
	}
	if (!has_n_option && ft_dprintf(fd_out, "\n") == -1)
		return (!SUCCESS);
	return (SUCCESS);
}

int	built_in_echo(t_msh *msh, char **cmd_with_args, int fd_out)
{
	(void) msh;
	if (handle_echo(cmd_with_args + 1, fd_out) != SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
