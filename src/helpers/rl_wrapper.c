#include "../minishell.h"

char *readline_wrapper(char *prompt)
{
	char *rl_raw;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		return (readline(prompt));
	rl_raw = get_next_line(STDIN_FILENO);
	if (rl_raw)
		rl_raw = ft_replace_char(rl_raw, '\n', '\0');
	return (rl_raw);
}
