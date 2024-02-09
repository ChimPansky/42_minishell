#include <signal.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

sig_atomic_t	g_sig_int_received;

// static void sig_int_handler(int signo)
// {
// 	g_signal_data.signal_code = signo;
// 	//ft_putstr_fd("SIG_INT_HANDLER", STDOUT_FILENO);
// 	if (signo != SIGINT)
// 		return ;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	write(g_signal_data.rl_pipe[1], "\n", 1); // write to readline to trigger end of input
// 	//rl_done = 1;
// 	//write(STDOUT_FILENO, "\n", 1);
// 	// rl_on_new_line();
// 	// rl_replace_line("", 1);
// 	// rl_redisplay();
// 	// //rl_done = 1;
// }

int	check_for_signals(t_msh *msh)
{
	if (g_sig_int_received)
	{
		msh->last_exit_code = EXIT_SIG_INT;
		g_sig_int_received = 0;
		return (true);
	}
	else
		return (false);
}

static void	sig_handler_rl_main(int signo)
{
	(void) signo;
	g_sig_int_received = true;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//write(g_signal_data.rl_pipe[1], "\n", 1); // write to readline to trigger end of input
}

static void	sig_handler_rl_heredoc(int signo)
{
	(void) signo;
	g_sig_int_received = true;
	//write(STDOUT_FILENO, "\n", 1);
	//rl_done = 1;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	//rl_forced_update_display();
	//rl_redisplay();
	//write(g_signal_data.rl_pipe[1], "\n", 1); // write to readline to trigger end of input
}

static void	sig_handler_no_rl(int signo)
{
	(void) signo;
	g_sig_int_received = true;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
}

int	configure_signals(t_signal_mode sig_mode)
{
	struct sigaction sig_act;

	sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
	if (sig_mode == SIG_READLINE_MAIN)
		sig_act.sa_handler = sig_handler_rl_main;
	else if (sig_mode == SIG_READLINE_HEREDOC)
		sig_act.sa_handler = sig_handler_rl_heredoc;
	else if (sig_mode == SIG_NO_READLINE)
		sig_act.sa_handler = sig_handler_no_rl;
	if (sigaction(SIGINT, &sig_act, NULL) != 0)
		return (ft_printf_err(
			"An error occurred while configuring signal handler.\n"),
			!SUCCESS);

	if (sig_mode != SIG_NO_READLINE)
	{
		sig_act.sa_handler = SIG_IGN;
		if (sigaction(SIGQUIT, &sig_act, NULL) != 0)
			return (ft_printf_err(
				"An error occurred while configuring signal handler.\n"),
				!SUCCESS);
	}
	return (SUCCESS);
}

// int register_signals(void)
// {
// 	struct sigaction sig_act;

// 	g_signal_data.signal_code = 0;
// 	if (pipe(g_signal_data.rl_pipe) == -1)
// 		return (perror("register_signals: rl_pipe"), !SUCCESS);
//     sigemptyset(&sig_act.sa_mask);
//     sig_act.sa_flags = 0;
// 	sig_act.sa_handler = SIG_IGN;
//     if (sigaction(SIGQUIT, &sig_act, NULL) != 0)
//     {
//         dprintf(STDERR_FILENO, "An error occurred while setting a signal handler.\n");
//         exit(EXIT_FAILURE);
//     }
//     sig_act.sa_handler = sig_int_handler;
//     if (sigaction(SIGINT, &sig_act, NULL) != 0)
//     {
//         dprintf(STDERR_FILENO, "An error occurred while setting a signal handler.\n");
//         exit(EXIT_FAILURE);
//     }
// 	return (SUCCESS);
// }
