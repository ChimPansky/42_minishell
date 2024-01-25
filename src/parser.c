/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/25 11:41:59 by vvilensk         ###   ########.fr       */
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
int 	parser(t_msh *msh)
{
    t_tokens    *cur_tokens;
    t_token     *token;
    char    **cmd_with_args;
    t_list  *redirections;

    cur_tokens = msh->tokens;
    token = NULL;
    cmd_with_args = NULL;
    redirections = NULL;
    if (!cur_tokens)
    {
        ft_putendl_fd("tokenlist empty!", STDERR_FILENO);
        return (1);
    }
    while (1)
    {
        if (cur_tokens)
            token = cur_tokens->content;
        if (!token)
            ft_putendl_fd("found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        // check tokens and add to command_list
        // at every | or at end of token list, add command_list
        if (!cur_tokens || token->tk_type == TK_PIPE)
        {
            command_add(&msh->commands, cmd_with_args, redirections);
            cmd_with_args = NULL;
            redirections = NULL;
            if (!cur_tokens)
                break;
        }
        else if (token->tk_type == TK_WORD)
            cmd_with_args = strings_append(cmd_with_args, token->word);
        else if (token->tk_type == TK_REDIR)
            redir_move(&redirections, token->redir);
        cur_tokens = cur_tokens->next;
    }
	//printf("printing commands...\n");
	//print_commands(&msh->commands);
	return (0);
}
