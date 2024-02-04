/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 18:23:20 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// mb add semicolon
bool    is_token_seperator(char c)
{
    const char  *shell_seps = "|<>&";

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
