/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/31 20:17:57 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



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

bool g_sigint_received;


int main_loop(t_msh *msh)
{
	char	*rl_chunk;
	char	*old_input;

	while(!msh->done)
	{
		// todo: remove as much as possible from t_msh
		//		free rl_chunk and rl_input (if not multiline) after each loop iteration
		// mb just have 2 variables here: rl_input and rl_input_multi...
		// incomplete (multiline) input like: echo hello|
		// --> stitch input to next input
		update_prompt(msh);
		if (msh->mult_line_input)
			rl_chunk = readline(msh->mult_line_prompt);
		else
			rl_chunk = readline(msh->prompt.buf);
		if (g_sigint_received)
		{
			msh->last_exit_code = 130;
			g_sigint_received = false;
		}
		if (!rl_chunk)
			ms_stop(msh);
		else
		{
			old_input = msh->rl_input;
			if (msh->rl_input)
			{
				msh->rl_input = ft_strjoin(msh->rl_input, rl_chunk);
				free(old_input);
			}
			else
				msh->rl_input = ft_strdup(rl_chunk);
			if (lex(msh, rl_chunk) == SUCCESS && msh->tokens)
				expand(msh);
			free(rl_chunk);
			if (msh->err_number)
				ms_error_msg(msh->err_number, msh->err_info);
			else if (!msh->mult_line_input && msh->tokens)
			{
				parse(msh);
				execute(msh, msh->commands);
			}
			if (msh->err_number || !msh->mult_line_input)
			{
				ft_lstclear(&msh->tokens, token_destroy);
				msh->last_token_type = TK_NULL;
				ft_lstclear(&msh->commands, destroy_command);
				msh->commands = NULL;
				if (msh->rl_input)
				{
					add_history(msh->rl_input);
					free_null((void **)&msh->rl_input);
				}
				msh->mult_line_input = false;
				msh->err_number = SUCCESS;
				msh->err_info = NULL;
			}
		}
	}
	return SUCCESS;
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
