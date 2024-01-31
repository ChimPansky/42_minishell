/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/31 14:26:26 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expander.h"

int			string_expand(t_msh *msh, t_string *str, bool *ws_expansion);
static void	handle_quote(int *quote_type,
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

	string_init(&expanded_str, "");
	to_expand = str->buf;
	quote_type = 0;
	while (*to_expand)
	{
		if (*to_expand == '\'' || *to_expand == '"')
			handle_quote(&quote_type, &to_expand, &expanded_str);
		else if (*to_expand == '$' && quote_type != '\'')
			handle_dollar(msh, ws_expansion, &to_expand, &expanded_str);
		else if (*to_expand == '~' && to_expand == str->buf)
			handle_tilde(msh, &to_expand, &expanded_str);
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
	string_destroy(&var_name);
	string_add_str(expanded, var_value);
	if (ws_expansion && ft_string_has_chars(var_value, STR_WHITESPACE))
		*ws_expansion = true;
	return (SUCCESS);
}

static int	handle_tilde(t_msh *msh, char **to_expand, t_string *expanded)
{
	(*to_expand)++;
	if (**to_expand == '\0' || **to_expand == '/'
		|| is_shell_space(**to_expand))
		string_add_str(expanded, var_get_value(msh->env, "HOME"));
	else
		string_add_chr(expanded, '~');
	return (SUCCESS);
}

static int	handle_dollar_question_mark(t_msh *msh, char **to_expand,
	t_string *expanded)
{
	char	*str_exit_code;

	(*to_expand)++;
	str_exit_code = ft_itoa(msh->last_exit_code);
	if (!str_exit_code)// MALLOC ERROR
		return (!SUCCESS);
	string_add_str(expanded, str_exit_code);
	free(str_exit_code);
	return (SUCCESS);
}
