/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:56:12 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "structures/list_tokens.h"
#include <unistd.h>

int	try_read_with_readline(t_msh *msh, t_string *rl_input)
{
	char	*rl_raw;

	rl_raw = readline_wrapper(msh->prompt.buf, false);
	if (!rl_raw)
	{
		if (errno)
			perror("readline");
		ms_stop(msh);
		return (!SUCCESS);
	}
	string_init_with_allocated(rl_input, rl_raw);
	return (SUCCESS);
}

int	main_loop(t_msh *msh)
{
	t_string	rl_input;
	t_tokenlist	*tokens;

	while (!msh->done)
	{
		update_prompt(msh);
		if (SUCCESS != try_read_with_readline(msh, &rl_input))
			continue ;
		check_for_signals(msh);
		if (SUCCESS != lex(msh, &tokens, rl_input.buf))
		{
			if (!check_for_signals(msh))
			{
				add_history(rl_input.buf);
				string_destroy(&rl_input);
			}
			continue ;
		}
		add_history(rl_input.buf);
		string_destroy(&rl_input);
		parse_and_execute(msh, tokens);
		check_for_signals(msh);
		tokenlist_destroy(&tokens);
	}
	return (SUCCESS);
}

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
