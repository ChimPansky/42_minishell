#include "../minishell.h"

char *readline_wrapper(char *prompt, int last_exit_code)
{
	char *rl_raw;

	(void)last_exit_code;

	if (isatty(STDIN_FILENO))
	{
		//if (last_exit_code == EXIT_SIG_INT)
		//	return (readline(NULL));
		//else
			return (readline(prompt));
	}
	rl_raw = get_next_line(STDIN_FILENO);
	if (rl_raw)
		rl_raw = ft_replace_char(rl_raw, '\n', '\0');
	return (rl_raw);
}
