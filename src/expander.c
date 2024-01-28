/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/28 20:36:22 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 	        expander(t_msh *msh);
static int      token_expand(t_msh *msh, t_token *token);
static int      string_expand(t_msh *msh, t_string *str);
//static void      token_remove_quotes(t_token *token);
//static int      *string_remove_quotes(t_string *str);
int    get_var_name(t_string *target, char **buffer);

//  expander: scans through token_list and looks for $-signs to expand
int 	expander(t_msh *msh)
{
    t_tokens    *cur_tokens;
    t_token     *token;

    cur_tokens = msh->tokens;
    token = NULL;
    if (!cur_tokens)
    {
        ft_putendl_fd("expander: tokenlist empty! (this should never happen)", STDERR_FILENO);
        return (1);
    }
    while (cur_tokens)
    {
        token = cur_tokens->content;
        if (!token)
            ft_putendl_fd("expander: found empty token in tokenlist! (this should never happen)", STDERR_FILENO);
        token_expand(msh, token); // ambiguous redirect error if expansion results in empty filename for redirections --> check in executor...
        token_remove_quotes(token);
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
        string_expand(msh, &token->string); // NULL check needed?
        if (!token->string.buf)
            return (!SUCCESS);
    }
    else if (token->tk_type == TK_REDIR)
    {
        string_expand(msh, &token->redir->string);   // NULL check needed? set errno?
        if (!token->redir->string.buf)
            return (!SUCCESS);
    }
    return (SUCCESS);
}

static int string_expand(t_msh *msh, t_string *str)
{
    t_string        old = *str;
    char            *old_buf;
    size_t          i;
    char            quote_type;
    t_string        var_name;

    old_buf = old.buf;
    i = 0;
    quote_type = 0;
    string_init(str, "");
    while(*(old.buf))
    {
        if ((quote_type == 0 && (*(old.buf) == '\'' || *(old.buf) == '"'))
            || (*(old.buf) == quote_type))
        {
            quote_type = *(old.buf);
            (old.buf)++;
        }
        else if (*(old.buf) == '$' && quote_type != '\'')
        {
            if (get_var_name(&var_name, &(old.buf) != SUCCESS))
                return (!SUCCESS);  //string_destroy(str)??
            if (!var_name.len)
                string_add_chr(str, '$');
            else
                string_add_str(str, var_get_value(msh->env, var_name.buf));
        }
        else
             string_add_chr(str, *(old.buf));
        (old.buf)++;
    }
    old.buf = old_buf;
    string_destroy(&old);
    msh->err_number = SUCCESS;  // WHY?
    return (SUCCESS);
}

// static void token_remove_quotes(t_token *token)
// {
//     if (token->tk_type == TK_WORD)
//         string_remove_quotes(token->string);
//     else if (token->tk_type == TK_REDIR && token->redir->type != FD_HEREDOC)    // Heredoc contents keep quotes apparently...
//         string_remove_quotes(token->redir->string);
// }

// static int    *string_remove_quotes(t_string *str)
// {
//     char    *cur_byte;
//     char    *last_byte;
//     char    quote_type;

//     cur_byte = str->buf;
//     last_byte = cur_byte + str->len;
//     quote_type = 0;
//     while (*cur_byte)
//     {
// 		if ((quote_type == 0 && (*cur_byte == '\'' || *cur_byte == '"'))
// 			|| (quote_type == *cur_byte))
// 		{
// 			if (quote_type == 0)
// 				quote_type = *cur_byte;
// 			else
// 				quote_type = *cur_byte;
// 			ft_memmove(cur_byte, cur_byte + 1, last_byte - cur_byte);
// 			last_byte--;
// 		}
//         cur_byte++;
//     }
//     str->len = ft_strlen(str->buf);
//     return (str);
// }

int    get_var_name(t_string *target, char **buffer)
{
    if (string_init(target, "") != SUCCESS)
        return (!SUCCESS);
    while (!is_var_separator(**buffer))
    {
        string_add_chr(target, **buffer);
        (*buffer)++;
    }
    return (SUCCESS);
}
