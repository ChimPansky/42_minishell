/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/14 17:40:05 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// for libft:
char **strings_append(char **strings, char *appendix)
{
	char	**new_strings;
	size_t	s_count;

	s_count = 0;
	if (!appendix)
		return (strings);
	while (strings[s_count])
		s_count++;
	new_strings = ft_calloc(s_count + 1, sizeof(char *));
	if (!new_strings)
		return (strings);
	s_count = 0;
	while (strings[s_count])
	{
		new_strings[s_count] = strings[s_count];
		s_count++;
	}
	new_strings[s_count] = ft_strdup(appendix);
	return (new_strings);
}

int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	char	*rl_chunk;
	char	*old_input;
	(void) ac;
	(void) av;

	init(&msh, envp);
	while(1)
	{
		// incomplete (multiline) input like: echo hello|
		// --> stitch input to next input
		if (msh.mult_line_input)
			rl_chunk = readline(msh.mult_line_prompt);
		else
			rl_chunk = readline(msh.prompt);
		if (!rl_chunk)
			ms_error(ER_READLINE);
		else
		{
			old_input = msh.rl_input;
			if (msh.rl_input)
			{
				msh.rl_input = ft_strjoin(msh.rl_input, rl_chunk);
				free(old_input);
			}
			else
				msh.rl_input = ft_strdup(rl_chunk);

			// if (CTRL+D)
			//		built_in_exit();
			// lexer: turns input into token_list; stores token_list in msh.tokens
			lexer(&msh, rl_chunk, &msh.tokens);
			if (rl_chunk)
				free(rl_chunk);
			// parser: takes list of tokens and turns it (with expansions) into list of one or several commands (=command chain)
			if (msh.err_syntax)
			{
				ms_error_msg(ER_UNEXPECTED_TOKEN, msh.unexpected_token);
				ft_lstclear(&msh.tokens, destroy_token);
				ft_lstclear(&msh.commands, destroy_command);
				add_history(msh.rl_input);
				free(msh.rl_input); // !!!!!!!!!!!!!!!!!!!1
				msh.rl_input = NULL;
				msh.mult_line_input = false;
				msh.err_syntax = false;
				msh.unexpected_token = NULL;
			}
			if (!msh.mult_line_input)
			{
				if (msh.tokens)
				{
					parser(&msh, &msh.tokens, msh.commands);
					execute(&msh, msh.commands);
					ft_lstclear(&msh.tokens, destroy_token);
					ft_lstclear(&msh.commands, destroy_command);
				}

				msh.last_token = NULL;
				if (msh.rl_input)
				{
					add_history(msh.rl_input);
					free(msh.rl_input); // !!!!!!!!!!!!!!!!!!!1
					msh.rl_input = NULL;
				}
				//free_null((void**)&msh.rl_input);
			}
		}
	}
}
