/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:47:22 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_string.h"

void	ms_init(t_msh *msh, char **envp)
{
	ft_bzero(msh, sizeof(t_msh));
	g_signal_no = 0;
	if (string_init_fixed_cap(&msh->prompt, PROMPT_MAX_LEN + 1) != SUCCESS)
		(perror("ms_init: string_init_fixed_cap"), exit(EXIT_FAILURE));
	msh->done = false;
	msh->last_exit_code = EXIT_SUCCESS;
	if (envp && envp[0])
	{
		msh->env = varlist_init_from_envp(envp);
		if (!msh->env)
			(string_destroy(&msh->prompt), exit(EXIT_FAILURE));
	}
}

int	check_for_signals(t_msh *msh)
{
	if (g_signal_no)
	{
		msh->last_exit_code = g_signal_no + 128;
		if (g_signal_no == SIGQUIT)
			write(STDOUT_FILENO, "\n", 1);
		g_signal_no = 0;
		return (true);
	}
	else
		return (false);
}

void	ms_destroy(t_msh *msh)
{
	rl_clear_history();
	string_destroy(&msh->prompt);
	varlist_destoy(&msh->env);
}

void	ms_stop(t_msh *msh)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_printf_err("\nexit\n");
	msh->done = true;
}
