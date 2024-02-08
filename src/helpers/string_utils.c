/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 13:20:02 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_string.h"

// mb add semicolon
bool    is_token_seperator(char c)
{
    const char  *shell_seps = "|<>&()";

    return (ft_isspace(c) || ft_strchr(shell_seps, c));
}

bool	is_special_var_name(char c)
{
    return (c == '?');
}

bool	is_var_name_start(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	if (is_special_var_name(c))
		return (true);
	return (false);
}

bool    is_var_separator(char c)
{
    const char  *var_seps = "\n$;&|><(){}[],.!?=+-*/%\\'\"";

    return (ft_isspace(c) || ft_strchr(var_seps, c));
}

size_t	str_remove_quotes(char **str)
{
	char	*pos_in_str;
	char	quote_type;
	bool	skip_char;

	pos_in_str = *str;
	quote_type = 0;
	while (*pos_in_str)
	{
		skip_char = false;
		if (quote_type == 0 && (*pos_in_str == '\'' || *pos_in_str == '"'))
		{
			quote_type = *pos_in_str;
			skip_char = true;
		}
		else if (quote_type == *pos_in_str)
		{
			quote_type = 0;
			skip_char = true;
		}
		if (skip_char)
			ft_memmove(pos_in_str, pos_in_str + 1, ft_strlen(pos_in_str + 1) + 1);
		else
			pos_in_str++;
	}
	return (ft_strlen(*str));
}

size_t	string_remove_quotes(t_string *string)
{
	size_t	new_len;

	new_len = str_remove_quotes(&string->buf);
	string->len = new_len;
	return (string->len);
}
