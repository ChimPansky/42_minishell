/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:04:32 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:28:15 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_unexpected_token(char *str)
{
	ft_printf_err("syntax error near unexpected token `%s'", str);
}

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
	else if (error_nr == ER_AMBIGUOUS_REDIRECT)
	{
		if (err_info)
		{
			ft_putstr_fd(err_info, STDERR_FILENO);
			ft_putendl_fd(": ", STDERR_FILENO);
		}
		ft_putendl_fd("ambiguous redirect", STDERR_FILENO);
	}
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
