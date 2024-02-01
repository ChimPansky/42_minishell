/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 14:42:51 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expander.h"

int			string_expand(t_msh *msh, t_string *str, bool *ws_expansion);
static int	handle_quote(int *quote_type,
				char **to_expand, t_string *expanded);
static int	handle_dollar(t_msh *msh, bool *ws_expansion,
				char **to_expand, t_string *expanded);
static int	handle_tilde(t_msh *msh, char **to_expand, t_string *expanded);
static int	handle_dollar_question_mark(t_msh *msh, char **to_expand,
				t_string *expanded);

int	string_expand(t_msh *msh, t_string *str, bool *ws_expansion)
{
	t_string	expanded_str;
	char		*to_expand;
	int			quote_type;

	if (string_init(&expanded_str, "") == !SUCCESS)
		return (!SUCCESS);
	to_expand = str->buf;
	quote_type = 0;
	while (*to_expand)
	{
		if (*to_expand == '\'' || *to_expand == '"')
		{
			if (handle_quote(&quote_type, &to_expand, &expanded_str) != SUCCESS)
				return (string_destroy(&expanded_str), !SUCCESS);
		}
		else if (*to_expand == '$' && quote_type != '\'')
		{
			if (handle_dollar(msh, ws_expansion, &to_expand, &expanded_str) != SUCCESS)
				return (string_destroy(&expanded_str), !SUCCESS);
		}
		else if (*to_expand == '~' && to_expand == str->buf)
		{
			if (handle_tilde(msh, &to_expand, &expanded_str) != SUCCESS)
				return (string_destroy(&expanded_str), !SUCCESS);
		}
		else
		{
			to_expand++;
			if (string_add_chr(&expanded_str, *(to_expand - 1)) != SUCCESS)
				return (string_destroy(&expanded_str), !SUCCESS);
		}
	}
	free(str->buf);
	str->buf = expanded_str.buf;
	return (SUCCESS);
}

static int	handle_quote(int *quote_type, char **to_expand, t_string *expanded)
{
	if (*quote_type == 0)
		*quote_type = **to_expand;
	else if (**to_expand == *quote_type)
		*quote_type = 0;
	else if (!string_add_chr(expanded, **to_expand))
		return (!SUCCESS);
	(*to_expand)++;
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
		return (string_add_chr(expanded, '$'));
	if (read_var_name(to_expand, &var_name) != SUCCESS)
		return (!SUCCESS);
	var_value = varlist_get_value(msh->env, var_name.buf);
	string_destroy(&var_name);
	if (ws_expansion && ft_string_has_chars(var_value, STR_WHITESPACE))
		*ws_expansion = true;
	return (string_add_str(expanded, var_value));
}

static int	handle_tilde(t_msh *msh, char **to_expand, t_string *expanded)
{
	(*to_expand)++;
	if (**to_expand == '\0' || **to_expand == '/'
		|| is_shell_space(**to_expand))
		return (string_add_str(expanded, varlist_get_value(msh->env, "HOME")));
	return (string_add_chr(expanded, '~'));
}

static int	handle_dollar_question_mark(t_msh *msh, char **to_expand,
	t_string *expanded)
{
	char	*str_exit_code;

	(*to_expand)++;
	str_exit_code = ft_itoa(msh->last_exit_code);
	if (!str_exit_code)
		return (!SUCCESS);
	if (string_add_str(expanded, str_exit_code) != SUCCESS)
		return (free(str_exit_code), !SUCCESS);
	free(str_exit_code);
	return (SUCCESS);
}
