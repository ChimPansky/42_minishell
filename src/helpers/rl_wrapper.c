/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 00:58:10 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:58:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*readline_wrapper(char *prompt, bool is_heredoc)
{
	char	*rl_raw;

	if (is_heredoc)
		configure_signals(SIG_READLINE_HEREDOC);
	else
		configure_signals(SIG_READLINE_MAIN);
	errno = 0;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		rl_raw = readline(prompt);
		configure_signals(SIG_NON_INTERACTIVE);
		return (rl_raw);
	}
	rl_raw = get_next_line(STDIN_FILENO);
	if (rl_raw)
		rl_raw = ft_replace_char(rl_raw, '\n', '\0');
	configure_signals(SIG_NON_INTERACTIVE);
	return (rl_raw);
}
