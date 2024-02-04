#include <signal.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static void sig_int_handler(int signo)
{
	if (signo != SIGINT)
		return ;
	rl_on_new_line();
	rl_replace_line("", 1);
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
	g_sigint_received = true;
}

void register_signals(void)
{
	struct sigaction sig_act;

	g_sigint_received = false;
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
	sig_act.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sig_act, NULL) != 0)
    {
        dprintf(STDERR_FILENO, "An error occurred while setting a signal handler.\n");
        exit(EXIT_FAILURE);
    }
    sig_act.sa_handler = sig_int_handler;
    if (sigaction(SIGINT, &sig_act, NULL) != 0)
    {
        dprintf(STDERR_FILENO, "An error occurred while setting a signal handler.\n");
        exit(EXIT_FAILURE);
    }
}
