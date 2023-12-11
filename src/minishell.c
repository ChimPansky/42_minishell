#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>

int	main()
{
	char 	*lines[5];
	int		i;

	i = 0;
	while(1)
	{
		// rl_on_new_line();
		// rl_replace_line()
		// rl_redisplay();
		lines[i] = readline("xxx: ");
		printf("%s\n", lines[i]);
		if (lines[i])
			add_history(lines[i]);

		// printf("rl_point: %d\n", rl_point);
		// printf("rl_end: %d\n", rl_end);
		// printf("rl_mark: %d\n", rl_mark);
		// printf("rl_done: %d\n", rl_done);
		// printf("rl_num_chars_to_read: %d\n", rl_num_chars_to_read);
		// printf("rl_erase: %d\n", rl_erase_empty_line);
		// printf("rl_terminal_name: %s\n", rl_terminal_name);
		i++;
	}
}
