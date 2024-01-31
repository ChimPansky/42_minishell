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
#include "ft_string.h"

void	ms_init(t_msh *msh, char **envp)
{
	register_signals();
	ft_bzero(msh, sizeof(t_msh));
	if (string_init_fixed_cap(&msh->prompt, PROMPT_MAX_LEN + 1) != SUCCESS)
		perror("init prompt"), exit(EXIT_FAILURE);
	msh->mult_line_prompt = ">";
	msh->done = false;
	if (envp && envp[0])
	{
		msh->env = vars_init_from_envp(envp);
		if (!msh->env)
			string_destroy(&msh->prompt), exit(EXIT_FAILURE);
	}
}

void	ms_destroy_and_exit(t_msh *msh)
{
	if (msh->rl_input)
		free_null((void **)&msh->rl_input);
	string_destroy(&msh->prompt);
	vars_destoy(&msh->env);
	exit(msh->last_exit_code);
}

void	ms_stop(t_msh *msh)
{
	ft_printf_err("exit\n");
	msh->done = true;
}
