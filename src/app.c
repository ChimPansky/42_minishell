/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:46:05 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 13:32:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_string.h"

int	ms_init(t_msh *msh, char **envp)
{
	//if (register_signals() != SUCCESS)
	//	return (!SUCCESS);
	if (pipe(g_signal_data.rl_pipe) == -1)
	 		return (perror("register_signals: rl_pipe"), !SUCCESS);
	ft_bzero(msh, sizeof(t_msh));
	ft_bzero(&g_signal_data, sizeof(t_signal_data));
	if (string_init_fixed_cap(&msh->prompt, PROMPT_MAX_LEN + 1) != SUCCESS)
		perror("init prompt"), exit(EXIT_FAILURE);
	msh->done = false;
	msh->last_exit_code = EXIT_SUCCESS;
	if (envp && envp[0])
	{
		msh->env = varlist_init_from_envp(envp);
		if (!msh->env)
			string_destroy(&msh->prompt), exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}

void	ms_destroy(t_msh *msh)
{
	rl_clear_history();
	string_destroy(&msh->prompt);
	varlist_destoy(&msh->env);
}

void	ms_stop(t_msh *msh)
{
	if (isatty(STDIN_FILENO))
		ft_printf_err("\nexit\n");
	msh->done = true;
}
