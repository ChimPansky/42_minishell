/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/30 19:05:00 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expander.h"

int 	        expander(t_msh *msh);
static int      token_expand(t_msh *msh, t_token *token);
static int string_expand(t_msh *msh, t_string *str, bool *ws_expansion);
static void	handle_quote(int *quote_type,
			char **to_expand, t_string *expanded);
static int	handle_dollar_question_mark(t_msh *msh, char **to_expand,
			t_string *expanded);
static int	handle_dollar(t_msh *msh, bool *ws_expansion,
	char **to_expand, t_string *expanded);
static int read_var_name(char **to_expand, t_string *var_name);
//static int    get_var_name(t_string *var_name_holder, char **to_expand);
//static void      token_remove_quotes(t_token *token);
//static int      *string_remove_quotes(t_string *str);

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
        //token_remove_quotes(token);
        cur_tokens = cur_tokens->next;
    }
	printf("printing tokens (after expansion)...\n");
	print_tokens(&msh->tokens);
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

static int string_expand(t_msh *msh, t_string *str, bool *ws_expansion)
{
	t_string	expanded_str;
	char		*to_expand;
	int			quote_type;

	string_init(&expanded_str, "");
	to_expand = str->buf;
	quote_type = 0;
	while (*to_expand)
	{
		if (*to_expand == '\'' || *to_expand == '"')
			handle_quote(&quote_type, &to_expand, &expanded_str);
		else if (*to_expand == '$' && quote_type != '\'')
			handle_dollar(msh, ws_expansion, &to_expand, &expanded_str);
		else
		{
			string_add_chr(&expanded_str, *to_expand);
			to_expand++;
		}
	}
	free(str->buf);
	str->buf = expanded_str.buf;

	return (SUCCESS);
}

static void	handle_quote(int *quote_type, char **to_expand, t_string *expanded)
{
	if (*quote_type == 0)
		*quote_type = **to_expand;
	else if (**to_expand == *quote_type)
		*quote_type = 0;
	else
		string_add_chr(expanded, **to_expand);
	(*to_expand)++;
}

static int read_var_name(char **to_expand, t_string *var_name)
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

static int	handle_dollar_question_mark(t_msh *msh, char **to_expand,
	t_string *expanded)
{
	char	*str_exit_code;

	(*to_expand)++;
	str_exit_code = ft_itoa(msh->last_exit_code);
	if (!str_exit_code)		// MALLOC ERROR
		return (!SUCCESS);
	string_add_str(expanded, str_exit_code);
	free(str_exit_code);
	return (SUCCESS);
}

static int	handle_dollar(t_msh *msh, bool *ws_expansion,
	char **to_expand, t_string *expanded)
{
	t_string	var_name;
	char		*var_value;

	(*to_expand)++;
	if (**to_expand == '?')
		return (handle_dollar_question_mark(msh, to_expand, expanded));
	else if (**to_expand == '\0' || is_var_separator(**to_expand))
		return (string_add_chr(expanded, '$'), SUCCESS);
	if (read_var_name(to_expand, &var_name) != SUCCESS)
		return (!SUCCESS);
	var_value = var_get_value(msh->env, var_name.buf);
	ft_putendl_fd(var_value, STDOUT_FILENO);
	string_destroy(&var_name);
	string_add_str(expanded, var_value);
	if (ws_expansion && ft_string_has_chars(var_value, STR_WHITESPACE))
		*ws_expansion = true;
	(*to_expand) += var_name.len;
	return (SUCCESS);
}
