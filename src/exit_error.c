/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:04:32 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 15:27:12 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_msg(int error_nr)
{
	if (error_nr == 1)
		ft_putendl_fd("Undefined Error.", STDERR_FILENO);
}

void	ms_error(int error_nr)
{
	if (errno)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error_nr)
		error_msg(error_nr);
	else
		ft_putstr_fd("\n", STDERR_FILENO);
}

void	ms_exit(t_msh *msh, int error_nr)
{
	if (error_nr)
		ms_error(error_nr);
	destroy(msh);
	if (error_nr)
		exit(error_nr);
	if (errno)
		exit(errno);
	exit(EXIT_SUCCESS);
}
