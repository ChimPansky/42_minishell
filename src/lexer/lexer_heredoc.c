/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 15:33:33 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
static int	process_heredoc(t_charptr_array *heredoc_content, char **limiter)
{
    char    	*line;
	t_string	line_w_nl;

	str_remove_quotes(limiter);
	line = NULL;
	while (1)
	{
		line = readline_wrapper("> ");
		if (line == NULL)
        {
            if (errno)
            	return (perror("readline heredoc"), !SUCCESS);
			ft_printf("\nwarning: here-document delimited "
				"by end-of-file (wanted '%s')\n", *limiter);
        }
		if (line == NULL || ft_strcmp(*limiter, line) == SUCCESS)
			break ;
		string_init_with_allocated(&line_w_nl, line);
		if (string_add_chr(&line_w_nl, '\n') != SUCCESS)
			return (string_destroy(&line_w_nl), !SUCCESS);
		if (charptr_array_add_allocated_str(heredoc_content, &(line_w_nl.buf))
			!= SUCCESS)
			return (!SUCCESS);
	}
    return (free(line), SUCCESS);
}

// newlines appear as ^J in rl_history.
// this is a limitation of the rl_history library and can not be changed...
// so for now we wont use this function...
// static int	add_heredoc_to_history(t_string *rl_mainloop_input,
// 	t_charptr_array heredoc_content)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < heredoc_content.sz)
// 	{
// 		if (string_add_chr(rl_mainloop_input, '\n') != SUCCESS
// 			|| string_add_str(rl_mainloop_input, heredoc_content.buf[i]) != SUCCESS)
// 			return (!SUCCESS);
// 		i++;
// 	}
// 	return (SUCCESS);
// }

// to add heredoc history add the function
// add_heredoc_to_history(rl_mainloop_input, token->redir->content))
// after process_heredoc
int 	lex_heredocs(t_tokenlist *tokens)
{
    t_tokenlist *cur_tokens;
    t_token     *token;

    cur_tokens = tokens;
    token = NULL;
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (token && token->tk_type == TK_REDIR
			&& token->redir->type == FD_HEREDOC)
		{
			if (process_heredoc(&token->redir->content,
				&(token->redir->string.buf)) != SUCCESS)
				return (!SUCCESS);
		}
        cur_tokens = cur_tokens->next;
    }
	return (SUCCESS);
}
