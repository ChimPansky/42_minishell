/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 02:22:13 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_string.h"

bool	is_token_seperator(char c)
{
	const char	*shell_seps = "|<>&()";

	return (ft_isspace(c) || ft_strchr(shell_seps, c));
}

bool	is_var_name_start(char c)
{
	const char	*special_var_names = "?";

	if (!c)
		return (false);
	if (ft_isalnum(c) || c == '_')
		return (true);
	if (ft_strchr(special_var_names, c) != NULL)
		return (true);
	return (false);
}

bool	is_var_separator(char c)
{
	const char	*var_seps = "\n$;&|><(){}[],.!?=+-*/%\\'\"";

	return (ft_isspace(c) || ft_strchr(var_seps, c));
}

size_t	str_remove_quotes(char **str)
{
	char	*str_pos;
	char	quote_type;
	bool	skip_char;

	str_pos = *str;
	quote_type = 0;
	while (*str_pos)
	{
		skip_char = false;
		if (quote_type == 0 && (*str_pos == '\'' || *str_pos == '"'))
		{
			quote_type = *str_pos;
			skip_char = true;
		}
		else if (quote_type == *str_pos)
		{
			quote_type = 0;
			skip_char = true;
		}
		if (skip_char)
			ft_memmove(str_pos, str_pos + 1, ft_strlen(str_pos + 1) + 1);
		else
			str_pos++;
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
