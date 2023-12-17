/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 15:34:19 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_msh *msh)
{
	msh->rl_input = NULL;
	msh->pwd[0] = '\0';
	msh->prompt = NULL;
	update_pwd(msh);
	update_prompt(msh);
	msh->envp = NULL;
	msh->in_fd = STDIN_FILENO;
	msh->out_fd = STDOUT_FILENO;
	msh->err_fd = STDERR_FILENO;
}
