#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>

sig_atomic_t g_signal_code;

static void sig_int_handler(int signo)
{
    g_signal_code = signo;
    write(1, "SIGINT\n", 7);
    rl_replace_line("", 0); // Clear the line
    rl_redisplay(); // Redisplay the prompt
    rl_done = 1;
    rl_clear_signals(); // Clear signal handler
    rl_forced_update_display(); // Force readline to return immediately
}

void register_signals(struct sigaction *sig_act)
{
    g_signal_code = 0;
    sig_act->sa_flags = 0;
    sig_act->sa_handler = &sig_int_handler;
    sigaction(SIGINT, sig_act, NULL);
}

int main()
{
    char    *rl_input;
    struct sigaction sig_act;

    register_signals(&sig_act);
    while (1)
    {
        rl_input = readline("gief: ");
        printf("line entered: %s\n", rl_input);
        if (rl_input && *rl_input == 'X') {
            free(rl_input); // Free the memory allocated by readline
            break;
        }
        free(rl_input); // Free the memory allocated by readline
    }

    return (0);
}
