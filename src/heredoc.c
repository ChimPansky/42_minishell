/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/30 13:50:40 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
int	process_here_doc(t_msh *msh, char **document, char *limiter)
{
    char    *line;
    char    *temp;

    *document = malloc(sizeof(char));
    if (!*document)
    {
        return (!SUCCESS);
    }
    **document = '\0';
	while (1)
	{
        // does it have \n in the end?
        // if no change strjoin to strnjoin with \n
		line = readline(msh->mult_line_prompt); // addhistory
		if (line == NULL)
        {
            ft_putstr_fd("readline returned NULL...\n", STDOUT_FILENO);
            if (errno)
                //errno...
                return !SUCCESS;
            else
            // ^D message and exit
                return !SUCCESS;
        }
		if (ft_strcmp(limiter, line) == MATCH)
			break ;
		temp = ft_strnjoin(3, *document, line, "\n");
        if (!temp)
            return(free(line), free(*document), !SUCCESS);
        (free(*document), free(line));
        *document = temp;
	}
        return (SUCCESS);
}

int 	read_heredocs(t_msh *msh, char **rl_chunk)
{
    t_tokens    *cur_tokens;
    t_token     *token;
    char        *old_chunk;

    cur_tokens = msh->tokens;
    token = NULL;
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (token && token->tk_type == TK_REDIR && token->redir.type == FD_HEREDOC)
        {
            old_chunk = *rl_chunk;
            process_here_doc(msh, &token->redir.string.buf, token->redir.string.buf);
            *rl_chunk = ft_strjoin(*rl_chunk, token->redir.string.buf);
            free(old_chunk);
        }
        cur_tokens = cur_tokens->next;
    }
	return (0);
}
