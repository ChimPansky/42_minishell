/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 19:08:30 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
static int	process_heredoc(t_charptr_array *heredoc_content, char *limiter)
{
    char    	*line;
	t_string	line_w_nl;

	line = NULL;
	while (1)
	{
		line = readline_wrapper("> ");
		if (line == NULL)
        {
            ft_putstr_fd("readline returned NULL...\n", STDOUT_FILENO);
            if (errno)
                //errno...
                return (!SUCCESS);
            else
            // ^D message and exit
                return (!SUCCESS);
        }
		if (ft_strcmp(limiter, line) == SUCCESS)
			break ;
		string_init_with_allocated(&line_w_nl, line);
		if (string_add_chr(&line_w_nl, '\n') != SUCCESS)
			return (string_destroy(&line_w_nl), !SUCCESS);
		if (charptr_array_add_allocated_str(heredoc_content, &(line_w_nl.buf))
			!= SUCCESS)
			return (!SUCCESS);
		//string_destroy(&line_w_nl);
	}
	free(line);
    return (SUCCESS);
}

static int	add_heredoc_to_history(t_string *rl_mainloop_input,
	t_charptr_array heredoc_content)
{
	size_t	i;

	i = 0;
	while (i < heredoc_content.sz)
	{//TODO: fix newlines appearing as ^J in rl_history...
		if (string_add_chr(rl_mainloop_input, '\n') != SUCCESS
			|| string_add_str(rl_mainloop_input, heredoc_content.buf[i]) != SUCCESS)
			return (!SUCCESS);
		i++;
	}
	return (SUCCESS);
}

int 	read_heredocs(t_tokenlist *tokens, t_string *rl_mainloop_input)
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
				token->redir->string.buf) != SUCCESS
				|| add_heredoc_to_history(rl_mainloop_input,
				token->redir->content) != SUCCESS)
				return (!SUCCESS);
		}
        cur_tokens = cur_tokens->next;
    }
	return (SUCCESS);
}
