/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/03 18:45:59 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
static int	process_heredoc(t_charptr_array *lines, char *limiter, t_string *rl_input)
{
    char    *line;

	line = NULL;
	while (1)
	{
		line = readline(">");
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
		if (string_add_chr(rl_input, '\n') != SUCCESS
			|| string_add_str(rl_input, line) != SUCCESS)
			return (!SUCCESS);
		if (ft_strcmp(limiter, line) == MATCH)
			break ;
		if (charptr_array_add_allocated_str(lines, &line) != SUCCESS)
			return (free(line), !SUCCESS);
	}
	free(line);
    return (SUCCESS);
}

int 	read_heredocs(t_tokenlist *tokens, t_string *rl_input)
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
				token->redir->string.buf, rl_input) != SUCCESS)
				return (!SUCCESS);
		}
        cur_tokens = cur_tokens->next;
    }
	return (SUCCESS);
}
