/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 07:22:53 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 13:14:36 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_builtin_pwd(t_minish *minish, char **cmd_with_args)
{
	(void)cmd_with_args;
	ms_update_pwd(minish);
	if (!minish->pwd)
		return (errno);
	printf("%s\n", minish->pwd);
	return (SUCCESS);
}

int	ms_builtin_exit(t_minish *minish, char **cmd_with_args)
{
	(void)cmd_with_args;
	ms_exit(minish, EXIT_SUCCESS);
	//ms_exit(minish, EXIT_FAILURE);
	return (SUCCESS);
}
