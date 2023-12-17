/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 15:43:22 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static bool	has_minus_n_option(char	**cmd_with_args)
{
	int		j;
	bool	minus_n;

	minus_n = false;
	if (!cmd_with_args[1] || cmd_with_args[1][0] != '-')
		return (false);
	j = 1;
	while ((cmd_with_args[1][j]))
	{
		if (cmd_with_args[1][j++] == 'n')
			minus_n = true;
		else
			return (false);
	}
	return (minus_n);
}

int	built_in_echo(t_msh *msh, char **cmd_with_args)
{
	bool	minus_n;
	int		i;

	minus_n = has_minus_n_option(cmd_with_args);
	(void)msh;
	i = 1;
	while (cmd_with_args[i])
	{
		if (i > 1 || ((i == 1) && !minus_n))
		{
			printf("%s", cmd_with_args[i]);
			if (cmd_with_args[i + 1])
				printf(" ");
		}
		i++;
	}
	if (!minus_n)
		printf("\n");
	return (SUCCESS);
}
