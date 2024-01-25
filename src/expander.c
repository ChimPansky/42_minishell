/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/25 11:46:42 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int expand_word(t_msh *msh, char **to_expand)
{
    char    *word;
    char    *expanded;
    size_t  i;

    if (!to_expand || !*to_expand)
        return (ERROR);
    if (!ft_strchr(*to_expand, '$'))
        return (SUCCESS);
    i = 0;
    word = *to_expand;
    expanded = NULL;
    while (word[i])
    {
        if (word[i] == '$')
        {

        }
        expanded = add_to_word(&expanded, word[i]);
        if (!expanded)  // Malloc Error...
            return (ERROR);
    }

    msh->err_number = SUCCESS;
    return (SUCCESS);
}

char    **get_word_to_expand(t_token *token)
{
    char    **to_expand;

    to_expand = NULL;
    if (token->tk_type == TK_WORD)
        to_expand = &token->word;
    else if (token->tk_type == TK_REDIR
        && token->redir && token->redir->type != FD_HEREDOC)
        to_expand = &token->redir->str;
    return (to_expand);
}
//  expander: scans through token_list and looks for $-signs to expand
int 	expander(t_msh *msh)
{
    t_tokens    *cur_tokens;
    t_token     *token;
    char        **to_expand;

    cur_tokens = msh->tokens;
    token = NULL;
    if (!cur_tokens)
    {
        ft_putendl_fd("tokenlist empty! (this should never happen)", STDERR_FILENO);
        return (1);
    }
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (!token)
            ft_putendl_fd("found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        to_expand = get_word_to_expand(token);
        if (to_expand)
        {
            if (expand_word(msh, to_expand) != SUCCESS) // ambiguous redirect error if expansion results in empty filename for redirections
                return (ERROR);
        }
        cur_tokens = cur_tokens->next;
    }
	//printf("printing tokens (after expansion)...\n");
	//print_tokens(&msh->tokens);
	return (0);
}
