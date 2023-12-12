/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:05:41 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/12 23:38:50 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(t_minish *minish, char *input)
{
	if (ft_strncmp(input, "pwd", ft_strlen("pwd")) == 0)
		printf("%s\n", getcwd(minish->pwd, PATH_MAX));
}

int	main()
{
	t_minish	minish;
	char		*input;

	ms_init(&minish);
	while(1)
	{
		ms_update(&minish);
		input = readline(minish.prompt);
		if (input)
		{
			//printf("%s\n", input);
			add_history(input);
			handle_input(&minish, input);
		}
	}
}
		// rl_on_new_line();
		// rl_replace_line()
		// rl_redisplay();

		// printf("rl_point: %d\n", rl_point);
		// printf("rl_end: %d\n", rl_end);
		// printf("rl_mark: %d\n", rl_mark);
		// printf("rl_done: %d\n", rl_done);
		// printf("rl_num_chars_to_read: %d\n", rl_num_chars_to_read);
		// printf("rl_erase: %d\n", rl_erase_empty_line);
		// printf("rl_terminal_name: %s\n", rl_terminal_name);
