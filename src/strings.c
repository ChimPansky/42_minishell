/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:29:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/22 11:30:19 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: MS_WHITESPACES AND MS_SPECIAL_CHARS are macros in ms_macros...
// should we store info somewhere else?
bool    is_shell_space(char c)
{
    if (c && ft_strchr(MS_WHITESPACES, c))
        return (true);
    return (false);
}

bool    is_shell_special(char c)
{
    if (c && ft_strchr(MS_SPECIAL_CHARS, c))
        return (true);
    return (false);
}

bool    is_word_sep(char c)
{
    if (!c || is_shell_space(c) || is_shell_special(c))
        return (true);
    return (false);
}

bool    is_var_separator(char c)
{
    return (is_word_sep(c));
}
