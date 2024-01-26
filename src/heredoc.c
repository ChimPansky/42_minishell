/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/26 17:21:49 by tkasbari         ###   ########.fr       */
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
		line = readline(msh->mult_line_prompt); // addhistory
        line_sz = ft_strlen(line);
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
        if (token && token->tk_type == TK_REDIR && token->redir->type == FD_HEREDOC)
        {
            old_chunk = *rl_chunk;
            process_here_doc(msh, &token->redir->word, token->redir->word);
            *rl_chunk = ft_strjoin(*rl_chunk, token->redir->word);
            free(old_chunk);
        }
        cur_tokens = cur_tokens->next;
    }
	return (0);
}
