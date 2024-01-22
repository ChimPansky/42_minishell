/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/22 11:29:54 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char    *get_var_name(char *word)
{
    char    *var_name;
    while (!is_var_separator(*word))
    {
        var_name = add_to_word(&var_name, *word);
        if (!var_name)
            return (NULL);
        word++;
    }
    return (var_name);
}

int expand_word(t_msh *msh, char **to_expand)
{
    char    *word;
    char    *expanded;
    t_var   var;
    size_t  i;

    ft_bzero(&var, sizeof(t_var));
    if (!to_expand || !*to_expand)
        return (ERROR);
    if (!ft_strchr(*to_expand, '$'))
        return (SUCCESS);
    i = 0;
    word = *to_expand;
    expanded = NULL;
    while (*word)
    {
        if (*word == '$')
        {
            var.name = get_var_name(word + 1);
            if (ft_strlen(var.name))
                var.value = var_get_value(msh->env, var.name);
            else
                expanded = add_to_word(&expanded, '$');

        }
        expanded = add_to_word(&expanded, word[i]);
        if (!expanded)  // Malloc Error...
            return (ERROR);
        word++;
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
	printf("printing tokens (after expansion)...\n");
	print_tokens(&msh->tokens);
	return (0);
}
