/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/31 14:43:34 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expander.h"

int			expander(t_msh *msh);
static int	token_expand(t_msh *msh, t_token *token);
int			read_var_name(char **to_expand, t_string *var_name);

//  expander: scans through token_list and looks for $-signs to expand
int	expander(t_msh *msh)
{
	t_tokens	*cur_tokens;
	t_token		*token;

	cur_tokens = msh->tokens;
	token = NULL;
	if (!cur_tokens)
	{
		ft_putendl_fd("expander: tokenlist empty! (this should never happen)",
			STDERR_FILENO);
		return (ERROR);
	}
	while (cur_tokens)
	{
        token = cur_tokens->content;
        if (!token)
            ft_putendl_fd("expander: found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        token_expand(msh, token);
        cur_tokens = cur_tokens->next;
    }
	//printf("printing tokens (after expansion)...\n");
	//print_tokens(&msh->tokens);
	return (0);
}

static int token_expand(t_msh *msh, t_token *token)
{
    if (token->tk_type == TK_WORD)
    {
        string_expand(msh, &token->string, NULL); // NULL check needed?
        if (!token->string.buf)
            return (!SUCCESS);
    }
    else if (token->tk_type == TK_REDIR)
    {
        string_expand(msh, &token->redir.string,
			&token->redir.whitespace_expansion);   // NULL check needed? set errno?
        if (!token->redir.string.buf)
            return (!SUCCESS);
    }
    return (SUCCESS);
}

int read_var_name(char **to_expand, t_string *var_name)
{
	if (string_init(var_name, "") != SUCCESS)
		return (!SUCCESS);
	while (!is_var_separator(**to_expand))
	{
		string_add_chr(var_name, **to_expand);
		(*to_expand)++;
	}
	return (SUCCESS);
}
