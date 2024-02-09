/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 00:46:07 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:49:26 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

sig_atomic_t	g_signal_no = 0;

static void	sig_handler_rl_main(int signo)
{
	g_signal_no = signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sig_handler_rl_heredoc(int signo)
{
	g_signal_no = signo;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

static void	sig_handler_non_interactive(int signo)
{
	g_signal_no = signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
}

static int	set_sigaction(struct sigaction *sig_act, int signo,
	void *handler_func)
{
	sig_act->sa_handler = handler_func;
	if (sigaction(signo, sig_act, NULL) != SUCCESS)
		return (perror("set_sigaction"), !SUCCESS);
	return (SUCCESS);
}

void	configure_signals(t_signal_mode sig_mode)
{
	struct sigaction	sig_act;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	if (sig_mode == SIG_READLINE_MAIN || sig_mode == SIG_READLINE_HEREDOC
		|| sig_mode == SIG_NON_INTERACTIVE)
		set_sigaction(&sig_act, SIGQUIT, SIG_IGN);
	if (sig_mode == SIG_READLINE_MAIN)
		set_sigaction(&sig_act, SIGINT, sig_handler_rl_main);
	else if (sig_mode == SIG_READLINE_HEREDOC)
		set_sigaction(&sig_act, SIGINT, sig_handler_rl_heredoc);
	else if (sig_mode == SIG_NON_INTERACTIVE)
		set_sigaction(&sig_act, SIGINT, sig_handler_non_interactive);
	else if (sig_mode == SIG_EXECUTOR)
	{
		set_sigaction(&sig_act, SIGINT, SIG_DFL);
		set_sigaction(&sig_act, SIGQUIT, SIG_DFL);
	}
}
