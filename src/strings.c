/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/22 13:24:55 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    is_shell_space(char c)
{
    const char  *whitespaces = " \t";

    return (c && ft_strchr(whitespaces, c));
}

bool    is_shell_seperator(char c)
{
    const char  *shell_seps = "|&<>;!";

    return (c && ft_strchr(shell_seps, c));
}

bool    is_word_sep(char c)
{
    return (!c || is_shell_space(c) || is_shell_seperator(c));
}
