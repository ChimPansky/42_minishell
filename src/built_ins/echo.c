/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/18 09:42:10 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static bool	has_n_flag(char	**cmd_with_args)
{
	int		j;
	bool	n_flag;

	n_flag = false;
	if (!cmd_with_args[1] || cmd_with_args[1][0] != '-')
		return (false);
	j = 1;
	while ((cmd_with_args[1][j]))
	{
		if (cmd_with_args[1][j++] == 'n')
			n_flag = true;
		else
			return (false);
	}
	return (n_flag);
}

int	built_in_echo(t_msh *msh, char **cmd_with_args)
{
	const bool	n_flag = has_n_flag(cmd_with_args);
	int		i;

	(void)msh;
	i = 1;
	while (cmd_with_args[i])
	{
		if (i > 1 || ((i == 1) && !n_flag))
		{
			printf("%s", cmd_with_args[i]);
			if (cmd_with_args[i + 1])
				printf(" ");
		}
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (SUCCESS);
}
