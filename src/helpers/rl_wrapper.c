#include "../minishell.h"

char *readline_wrapper(char *prompt)
{
	char *rl_raw;

	if (ft_strcmp(prompt, PROMPT_HEREDOC) == SUCCESS)
		configure_signals(SIG_READLINE_HEREDOC);
	else
		configure_signals(SIG_READLINE_MAIN);
	if (isatty(STDIN_FILENO))
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
