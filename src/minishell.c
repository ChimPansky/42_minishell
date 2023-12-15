/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 14:15:52 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	t_msh	msh;
	ms_init(&msh);
	while(1)
	{
		ms_update(&msh);
		msh.rl_input = readline(msh.prompt);
		if (msh.rl_input)
		{
			//printf("%s\n", input);
			parse(&msh, msh.rl_input);
			add_history(msh.rl_input);
			free(msh.rl_input);
		}
		else
			ms_exit(&msh, EXIT_FAILURE);
	}
}
