/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/23 16:12:42 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
int	process_here_doc(char **document, char *limiter)
{
    char *line;
    char *temp;
    size_t line_sz;
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
		line = readline(NULL); // addhistory
        line_sz = ft_strlen(line);
		if (line == NULL)
        {
            if (errno)
                //errno...
                return !SUCCESS;
            else
            // ^D message and exit
                return !SUCCESS;
        }
		if (ft_strncmp(limiter, line, line_sz - 1) == MATCH)
			break ;
		temp = ft_strjoin(*document, line);
        if (!temp)
            return(free(line), free(*document), !SUCCESS);
        (free(*document), free(line));
        *document = temp;
	}
        return (SUCCESS);
}

int 	read_heredocs(t_msh *msh)
{
    t_tokens    *cur_tokens;
    t_token     *token;

    cur_tokens = msh->tokens;
    token = NULL;
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (token && token->tk_type == TK_REDIR && token->redir->type == FD_HEREDOC)
            process_here_doc(&token->redir->str, token->redir->str);
        cur_tokens = cur_tokens->next;
    }
	return (0);
}
