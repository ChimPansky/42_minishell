/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/23 18:25:56 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *remove_quotes(char **str)
{
    char    *cur_byte;
    char    *last_byte;
    char    quote_type;

    cur_byte = *str;
    last_byte = cur_byte + ft_strlen(*str);
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
    return (*str);
}

bool    is_shell_space(char c)
{
    const char  *whitespaces = " \t";

    return (c && ft_strchr(whitespaces, c));
}

bool    is_token_seperator(char c)
{
    const char  *shell_seps = "|<>;!";

    return (is_shell_space(c) || ft_strchr(shell_seps, c));
}


bool    is_var_separator(char c)
{
    const char  *var_seps = "$;&|><(){}[],.!?=+-*/%\\'\"";

    return (is_shell_space(c) || ft_strchr(var_seps, c));
}
