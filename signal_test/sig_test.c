/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/07 10:49:09 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <errno.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

# define EXIT_SIG_INT				130
# define SUCCESS					0

bool g_sigint_received;
bool done;
int	exit_code;


// todo: find all the places where to check for sigint:
// 	minimum main readline, heredoc readline, executor
int	check_for_sigint()
{
	if (g_sigint_received)
	{
	// differentiate between ctrl-c and ctrl-d:
	if (g_sigint_received && !errno)// ctrl-d
		return (SUCCESS);
	else if (g_sigint_received && errno)// ctrl-c
		exit_code = EXIT_SIG_INT;
		g_sigint_received = false;
		return (!SUCCESS);
	}
	return (SUCCESS);
}
void	handle_input(char *rl_input)
{
	printf("rl_input: %s\n", rl_input);
	printf("exit_code: %d\n", exit_code);
	printf("done: %d\n", done);
	printf("g_sigint_received: %d\n", g_sigint_received);
}

static void sig_int_handler(int signo)
{
	if (signo != SIGINT)
		return ;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 1);
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
int main_loop()
{
	char	*rl_input;
	while(!done)
	{
		errno = 0;
		rl_input = readline(">>> ");
		if (!rl_input)
		{
			if (SUCCESS != check_for_sigint())
				continue;
			else
				done = true;
			continue;
		}

		handle_input(rl_input);
		free(rl_input);
	}
	return (SUCCESS);
}

// add parameter check? are we allowed to call for example: ./minishell arg1 arg2...
int	main(int ac, char **av, char **envp)
{
	g_sigint_received = false;
	done = false;
	exit_code = 0;

	(void) ac;
	(void) av;
	register_signals();
	main_loop();
	return (exit_code);
}
