/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_error_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:04:32 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/12 23:18:29 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_error(char *err_msg)
{
	if (errno)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
}

void	ms_exit(t_minish *minish)
{
	//ms_destroy(minish);
	(void)minish;
	if (errno)
		exit(errno);
	else
		exit(EXIT_FAILURE);
}

void	ms_error_exit(t_minish *minish, char *err_msg)
{
	ms_error(err_msg);
	ms_exit(minish);
}
