/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:04:32 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:34:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_unexp_tk_s(t_msh *msh, char *token)
{
	if (*token == '\n')
		ft_printf_err("syntax error near unexpected token 'newline'\n");
	else
		ft_printf_err("syntax error near unexpected token '%s'\n", token);
	msh->last_exit_code = EXIT_UNEXPECTED_TOKEN;
}

void	error_unexp_tk_c(t_msh *msh, char symbol)
{
	if (symbol == '\n' || !symbol)
		ft_printf_err("syntax error near unexpected token 'newline'\n");
	else
		ft_printf_err("syntax error near unexpected token '%c'\n", symbol);
	msh->last_exit_code = EXIT_UNEXPECTED_TOKEN;
}
