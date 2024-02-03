/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/03 12:05:58 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool g_sigint_received;

int try_read_with_readline(t_msh *msh, t_string *rl_input)
{
	char	*rl_raw;

	rl_raw = readline(msh->prompt.buf);
	if (!rl_raw)
	{
		if (errno) // check for type of err, sometimes ms_stop
			perror("readline");
		else
			ms_stop(msh);
		return (!SUCCESS);
	}
	string_init_with_allocated(rl_input, rl_raw);
	return (SUCCESS);
}

int	check_for_sigint(t_msh *msh)
{
	if (g_sigint_received)
	{
		msh->last_exit_code = 130;
		g_sigint_received = false;
		return (!SUCCESS);
	}
	return (SUCCESS);
}

int main_loop(t_msh *msh)
{
	t_string 		rl_input;
	t_tokenlist		*tokens;

	while(!msh->done)
	{
		errno = 0;
		update_prompt(msh);
		if (SUCCESS != try_read_with_readline(msh, &rl_input))
			continue;
		if (SUCCESS != check_for_sigint(msh))
		{
			string_destroy(&rl_input);
			continue;
		}
		if (SUCCESS != lex(msh, &tokens, rl_input.buf) || !tokens)
		{
			add_history(rl_input.buf), string_destroy(&rl_input);
			continue;
		}
		if (SUCCESS != read_heredocs(tokens, &rl_input))
		{
			(add_history(rl_input.buf), string_destroy(&rl_input), tokenlist_destroy(&tokens));
			continue;
		}
		add_history(rl_input.buf);
		parse_and_execute(msh, tokens);
	}
	return (SUCCESS);
}

// add parameter check? are we allowed to caall for example: ./minishell arg1 arg2...
int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	(void) ac;
	(void) av;

	ms_init(&msh, envp);
	main_loop(&msh);
	ms_destroy_and_exit(&msh);
}
