#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>

sig_atomic_t g_signal_code;


int g_pipe_fd[2];

// static void sig_int_handler(int signo)
// {
//     write(g_pipe_fd[1], "\n", 1); // Write to pipe
// }

static void sig_int_handler(int signo)
{
	g_signal_code = signo;
	//write(1, "SIGINT\n", 7);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(g_pipe_fd[1], "x", 1); // Write to pipe
	//rl_done = 1;
	//printf("\n");
	//fflush(stdout);
	//rl_clear_signals();
	//fflush(stdin);

	/* this works:
	rl_replace_line("", 0);
	rl_forced_update_display(); */

}

void register_signals(struct sigaction *sig_act)
{
	if (pipe(g_pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	g_signal_code = 0;
	sig_act->sa_flags = SA_RESTART;
	sig_act->sa_handler = &sig_int_handler;
	sigaction(SIGINT, sig_act, NULL);

}


int main()
{
	char	*rl_input;
	struct sigaction sig_act;

	register_signals(&sig_act);
	while (1)
	{
		rl_input = readline("gief: ");
		printf("line entered: %s\n", rl_input);
		if (*rl_input == 'X')
			break;
	}

	return (0);
}
