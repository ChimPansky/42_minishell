/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/06 14:07:37 by tkasbari         ###   ########.fr       */
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
			//parse(&msh, msh.rl_input);
			new_parse(&msh, msh.tokens, msh.rl_input);
			add_history(msh.rl_input);
			ft_lstclear(&msh.tokens, destroy_token);
			free(msh.rl_input);
		}
		else
		{
			perror(NULL);
			ms_exit(&msh, EXIT_SUCCESS);
		}
	}
}
