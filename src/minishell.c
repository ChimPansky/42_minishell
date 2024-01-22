/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/22 13:03:04 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// for libft (to append char* to char**):
char **strings_append(char **strings, char *appendix)
{
	char	**new_strings;
	size_t	s_count;

	s_count = 0;
	if (!appendix)
		return (strings);
	while (strings && strings[s_count])
		s_count++;
	new_strings = ft_calloc(s_count + 2, sizeof(char *));
	if (!new_strings)
		return (strings);
	s_count = 0;
	while (strings && strings[s_count])
	{
		new_strings[s_count] = strings[s_count];
		s_count++;
	}
	new_strings[s_count] = ft_strdup(appendix);
	new_strings[s_count + 1] = NULL;
	return (new_strings);
}
// for libft (to print out char**):
void str_print(char **strings)
{
	int i;
	i = 0;

	if (!strings)
		printf("{NULL}\n");
	while (strings && strings[i])
	{
		printf("%d: %s\n", i, strings[i]);
		i++;
	}
	printf("%d: %s\n", i, "NULL");
}
// for libft (add 1 char to a string):
char    *add_to_word(char **word, char new_char)
{
	char	*to_free;
    char    appendix[2];

	if (!new_char)
		return (*word);
    appendix[0] = new_char;
    appendix[1] = '\0';
    if (!*word)
		*word = ft_strdup(appendix);
    else
    {
		to_free = *word;
        *word = ft_strjoin(*word, appendix);
        free(to_free);
    }
    return (*word);
}

int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	char	*rl_chunk;
	char	*old_input;
	(void) ac;
	(void) av;

	// add parameter check? are we allowed to caall for example: ./minishell arg1 arg2...
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
			// expander: scans through token_list and looks for $-signs to expand
			// parser: takes list of tokens and turns it (with expansions) into list of one or several commands (=command chain)
			// executor: takes list of commands command chain and executes them (piping them together); Bonus: executor also has to be able to logically connect commands (&&, || )
			if (lexer(&msh, rl_chunk) == SUCCESS)
				expander(&msh);
			if (rl_chunk)
				free(rl_chunk);
			if (msh.err_number)
				ms_error_msg(msh.err_number, msh.err_info);
			else if (!msh.mult_line_input && msh.tokens)
			{
				parser(&msh);
				execute(&msh, msh.commands);
			}
			if (msh.err_number || !msh.mult_line_input)
			{
				ft_lstclear(&msh.tokens, destroy_token);
				msh.last_token = NULL;
				ft_lstclear(&msh.commands, destroy_command);
				msh.commands = NULL;
				if (msh.rl_input)
				{
					add_history(msh.rl_input);
					free_null((void **)&msh.rl_input);
				}
				msh.mult_line_input = false;
				msh.err_number = SUCCESS;
				msh.err_info = NULL;
			}
		}
	}
}
