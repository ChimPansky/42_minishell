/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/18 16:32:47 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_msh *msh, char **envp)
{
	ft_bzero(msh, sizeof(t_msh));
	msh->mult_line_prompt = ">";
	update_pwd(msh);
	update_prompt(msh);
	if (envp && envp[0])
	{
		msh->env = vars_init_from_envp(envp);
		if (!msh->env)
			exit(EXIT_FAILURE);
	}
	msh->in_fd = STDIN_FILENO;
	msh->out_fd = STDOUT_FILENO;
	msh->err_fd = STDERR_FILENO;
}
