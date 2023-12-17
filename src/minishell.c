/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 15:33:06 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	(void) ac;
	(void) av;

	init(&msh, envp);
	while(1)
	{
		//update(&msh);
		msh.rl_input = readline(msh.prompt);
		if (msh.rl_input)
		{
			// if (CTRL+D)
			//		built_in_exit();
			parse(&msh, msh.rl_input);
			add_history(msh.rl_input);
			free(msh.rl_input);
		}
		else
			ms_exit(&msh, EXIT_FAILURE);
	}
}
