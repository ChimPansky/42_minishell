/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/26 17:12:12 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 	        expander(t_msh *msh);
static int      token_expand(t_msh *msh, t_token *token);
static t_string *string_expand(t_msh *msh, char *word);
static void      token_remove_quotes(t_token *token);
static t_string *string_remove_quotes(t_string *str);
static t_string *get_var_name(char *word);

//  expander: scans through token_list and looks for $-signs to expand
int 	expander(t_msh *msh)
{
    t_tokens    *cur_tokens;
    t_token     *token;
    t_string    *to_expand;

    cur_tokens = msh->tokens;
    token = NULL;
    to_expand = NULL;
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
	return (0);
}

static int token_expand(t_msh *msh, t_token *token)
{
    if (token->tk_type == TK_WORD)
        token->string = string_expand(msh, token->word); // NULL check needed?
    else if (token->tk_type == TK_REDIR)
        token->redir->string = string_expand(msh, token->redir->word);   // NULL check needed? set errno?
    if (!token->string || !token->redir->string)
        return (!SUCCESS);
    return (SUCCESS);
}

static t_string *string_expand(t_msh *msh, char *word)
{
    t_string    *expanded;
    t_string    *var_name;
    char        *var_value;

    if (!word || !ft_strchr(word, '$'))
        return (string_create(word));
    expanded = string_create("");
    if (!expanded)
        return (NULL);
    while (*word)
    {
        if (*word == '$')
        {
            var_name = get_var_name(word + 1);
            if (!var_name)
                return (string_destroy(expanded), NULL);
            if (ft_strlen(var_name->buf))
            {
                var_value = var_get_value(msh->env, var_name->buf);
                string_add_str(expanded, var_value);
                word += ft_strlen(var_name->buf);
                string_destroy(var_name);
            }
            else
                string_add_chr(expanded, '$');
        }
        else
            string_add_chr(expanded, *word);
        word++;
    }
    msh->err_number = SUCCESS;
    return (expanded);
}

static void token_remove_quotes(t_token *token)
{
    if (token->tk_type == TK_WORD)
        string_remove_quotes(token->string);
    else if (token->tk_type == TK_REDIR && token->redir->type != FD_HEREDOC)    // Heredoc contents keep quotes apparently...
        string_remove_quotes(token->redir->string);
}

static t_string    *string_remove_quotes(t_string *str)
{
    char    *cur_byte;
    char    *last_byte;
    char    quote_type;

    cur_byte = str->buf;
    last_byte = cur_byte + str->len;
    quote_type = 0;
    while (*cur_byte)
    {
		if ((quote_type == 0 && (*cur_byte == '\'' || *cur_byte == '"'))
			|| (quote_type == *cur_byte))
		{
			if (quote_type == 0)
				quote_type = *cur_byte;
			else
				quote_type = *cur_byte;
			ft_memmove(cur_byte, cur_byte + 1, last_byte - cur_byte);
			last_byte--;
		}
        cur_byte++;
    }
    str->len = ft_strlen(str->buf);
    return (str);
}

t_string    *get_var_name(char *word)
{
    t_string    *str;

    str = string_create("");
    if (!str)
        return (NULL);
    while (!is_var_separator(*word))
    {
        string_add_chr(str, *word);
        word++;
    }
    return (str);
}
