/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 15:44:39 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_msh *msh, char **envp)
{
	ft_bzero(msh, sizeof(t_msh));
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
	// char** a = vars_convert_to_array(minish->env);
	// while (*a)
	// {
	// 	printf("%s\n", *a++);
	// }
}