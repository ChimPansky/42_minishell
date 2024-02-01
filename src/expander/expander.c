/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 14:32:15 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expander.h"

int			expander(t_msh *msh);
static int	token_expand(t_msh *msh, t_token *token);
int			read_var_name(char **to_expand, t_string *var_name);

//  expander: scans through token_list and looks for $-signs to expand
int	expand(t_msh *msh, t_tokenlist *tokens)
{
	t_tokenlist	*cur_token;
	t_token		*token;

	cur_token = tokens;
	token = NULL;
	if (!cur_token)
	{
		ft_putendl_fd("expander: tokenlist empty! (this should never happen)",
			STDERR_FILENO);
		return (!SUCCESS);
	}
	while (cur_token)
	{
        token = cur_token->content;
        if (!token)
            ft_putendl_fd("expander: found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        if (SUCCESS != token_expand(msh, token))
			return (!SUCCESS);
        cur_token = cur_token->next;
    }
	//printf("printing tokens (after expansion)...\n");
	//print_tokens(&msh->tokens);
	return (SUCCESS);
}

static int token_expand(t_msh *msh, t_token *token)
{
    if (token->tk_type == TK_WORD)
        return (string_expand(msh, &token->string, NULL));
    else if (token->tk_type == TK_REDIR)
        return (string_expand(msh, &token->redir.string,
			&token->redir.whitespace_expansion));
    return (SUCCESS);
}

int read_var_name(char **to_expand, t_string *var_name)
{
	if (string_init(var_name, "") != SUCCESS)
		return (!SUCCESS);
	while (!is_var_separator(**to_expand))
	{
		if (string_add_chr(var_name, **to_expand) != SUCCESS)
			return (string_destroy(var_name), !SUCCESS);
		(*to_expand)++;
	}
	return (SUCCESS);
}
