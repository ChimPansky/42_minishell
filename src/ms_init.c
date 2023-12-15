/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/12 22:19:43 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_init(t_msh *minish, char **envp)
{
	ft_bzero(minish, sizeof(t_msh));
	if (envp && envp[0])
	{
		minish->env = vars_init_from_envp(envp);
		if (!minish->env)
			exit(EXIT_FAILURE);
	}
	minish->in_fd = STDIN_FILENO;
	minish->out_fd = STDOUT_FILENO;
	minish->err_fd = STDERR_FILENO;
	char** a = vars_convert_to_array(minish->env);
	while (*a)
	{
		printf("%s\n", *a++);
	}
}
