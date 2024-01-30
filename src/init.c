/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/27 22:26:18 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_msh *msh, char **envp)
{
	register_signals();
	ft_bzero(msh, sizeof(t_msh));
	msh->mult_line_prompt = ">";
	if (envp && envp[0])
	{
		msh->env = vars_init_from_envp(envp);
		if (!msh->env)
			exit(EXIT_FAILURE);
	}
}
