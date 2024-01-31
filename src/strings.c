/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/30 15:19:51 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    is_shell_space(char c)
{
    const char  *whitespaces = STR_WHITESPACE;

    return (c && ft_strchr(whitespaces, c));
}

bool    is_token_seperator(char c)
{
    const char  *shell_seps = "|<>;!";

    return (is_shell_space(c) || ft_strchr(shell_seps, c));
}


bool    is_var_separator(char c)
{
    const char  *var_seps = "\n$;&|><(){}[],.!?=+-*/%\\'\"";

    return (is_shell_space(c) || ft_strchr(var_seps, c));
}
