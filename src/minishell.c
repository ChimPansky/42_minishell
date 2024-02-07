/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/07 10:54:18 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "structures/list_tokens.h"
#include <errno.h>

bool g_sigint_received;

// todo: find all the places where to check for sigint:
// 	minimum main readline, heredoc readline, executor
static int	check_for_sigint(t_msh *msh)
{
	if (g_sigint_received)
	{
		msh->last_exit_code = EXIT_SIG_INT;
		g_sigint_received = false;
		return (!SUCCESS);
	}
	return (SUCCESS);
}

static int try_read_with_readline(t_msh *msh, t_string *rl_input)
{
	char	*rl_raw;

	rl_raw = readline_wrapper(msh->prompt.buf);
	if (!rl_raw)
	{
		if (!errno)
			ms_stop(msh);
		else if (check_for_sigint(msh) != SUCCESS)
			return (!SUCCESS);
		else // check for type of err, sometimes ms_stop
			perror("readline"), ms_stop(msh);
		return (!SUCCESS);
	}
	string_init_with_allocated(rl_input, rl_raw);
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
		if (SUCCESS != lex(msh, &tokens, rl_input.buf))
		{
			add_history(rl_input.buf);
			string_destroy(&rl_input);
			continue;
		}
		//print_tokens(tokens);
		add_history(rl_input.buf);
		string_destroy(&rl_input);
		parse_and_execute(msh, tokens);
		tokenlist_destroy(&tokens);
	}
	return (SUCCESS);
}

// add parameter check? are we allowed to call for example: ./minishell arg1 arg2...
int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	int		exit_code;

	(void) ac;
	(void) av;
	ms_init(&msh, envp);
	main_loop(&msh);
	exit_code = msh.last_exit_code;
	ms_destroy(&msh);
	return (exit_code);
}
