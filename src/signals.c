#include <signal.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

sig_atomic_t g_signal_received;

static void sig_int_handler(int signo)
{
	g_signal_received = signo;
	ft_putstr_fd("SIG_INT_HANDLER", STDOUT_FILENO);
	if (signo != SIGINT)
		return ;
	//write(STDOUT_FILENO, "\n", 1);
	//rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
	//rl_redisplay();

	//write(STDOUT_FILENO, "\n", 1);
	// rl_on_new_line();
	// rl_replace_line("", 1);
	// rl_redisplay();
	// //rl_done = 1;
}

void register_signals(void)
{
	struct sigaction sig_act;

	g_signal_received = false;
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
