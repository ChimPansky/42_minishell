/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:32:45 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_detail	*redir_move(t_redirections **redirections, t_redir_detail *redir)
{
	if (!redir)
		return NULL;
	t_redirections *new_redir = ft_lstnew(redir);
	if (!new_redir)
		return (NULL);
	ft_lstadd_back(redirections, new_redir);
	return (redir);
}


// takes list of tokens and turns it into list of one or several commands (command chain); Also: perform variable expansions...
int 	parse(t_msh *msh, t_tokenlist *tokens, t_commandlist **commands_p)
{
    t_tokenlist *cur_token;
    t_token     *token;
    char        **cmd_with_args;
    t_list      *redirections;

    (void)msh;
    *commands_p = NULL;
    cur_token = tokens;
    token = NULL;
    cmd_with_args = NULL;
    redirections = NULL;
    while (1)
    {
        if (cur_token)
            token = cur_token->content;
        if (!token)
            ft_putendl_fd("found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        if (!cur_token || token->tk_type == TK_PIPE)
        {
            if (!command_add(commands_p, cmd_with_args, redirections))
				return (perror("parse"), commandlist_destroy(commands_p),  !SUCCESS);
            command_add(commands_p, cmd_with_args, redirections);
            cmd_with_args = NULL;
            redirections = NULL;
            if (!cur_token)
                break;
        }
        else if (token->tk_type == TK_WORD)
            cmd_with_args = strings_append(cmd_with_args, token->string.buf);
        else if (token->tk_type == TK_REDIR)
            redir_move(&redirections, &token->redir);
        cur_token = cur_token->next;
    }
	// printf("printing commands...\n");
	// print_commands(&msh->commands);
	return (SUCCESS);
}
