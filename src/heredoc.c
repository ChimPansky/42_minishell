/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:08:11 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:27:34 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
static int	process_here_doc(char **document, char *limiter, t_string *rl_input)
{
    char    *line;
    char    *temp;
    //t_string    doc;
    (void)rl_input;

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
		line = readline(">"); // addhistory
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

int 	read_heredocs(t_tokenlist *tokens, t_string *rl_input)
{
    t_tokenlist *cur_tokens;
    t_token     *token;

    cur_tokens = tokens;
    token = NULL;
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (token && token->tk_type == TK_REDIR && token->redir.type == FD_HEREDOC)
        {
            process_here_doc(&token->redir.string.buf, token->redir.string.buf, rl_input);
            //string_add_str(rl_input, token->redir.)
            //*rl_input = ft_strjoin(*rl_input, token->redir.string.buf);
            // free(old_input);
        }
        cur_tokens = cur_tokens->next;
    }
	return (0);
}
