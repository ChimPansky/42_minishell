/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 12:55:14 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_init(t_msh *minish)
{
	minish->rl_input = NULL;
	minish->pwd[0] = '\0';
	minish->prompt = NULL;
	minish->envp = NULL;
	minish->in_fd = STDIN_FILENO;
	minish->out_fd = STDOUT_FILENO;
	minish->err_fd = STDERR_FILENO;
}
