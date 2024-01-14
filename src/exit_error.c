/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:04:32 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/14 14:14:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_error_msg(int error_nr, char *err_info)
{

	if (error_nr == ER_UNDEFINED)
		ft_putendl_fd("Undefined Error.", STDERR_FILENO);
	else if (error_nr == ER_QUOTES)
		ft_putendl_fd("Error: Unclosed quotes.", STDERR_FILENO);
	else if (error_nr == ER_UNEXPECTED_TOKEN)
	{
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		if (err_info)
		{
			ft_putstr_fd(err_info, STDERR_FILENO);
			ft_putendl_fd("'", STDERR_FILENO);
		}
		else
			ft_putendl_fd("unknown token'", STDERR_FILENO);
	}
	else if (error_nr == ER_READLINE)
		ft_putendl_fd("Readline error.", STDERR_FILENO);
	else if (error_nr == ER_MALLOC)
		ft_putendl_fd("Memory Allocation Error.", STDERR_FILENO);
}

void	ms_error(int error_nr)
{
	if (errno)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error_nr)
		ms_error_msg(error_nr, NULL);
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
