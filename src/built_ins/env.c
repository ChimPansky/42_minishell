/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 13:21:20 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_env(t_msh *msh, char **cmd_with_args, int fd_out)
{
	t_varlist	*env;
	t_var		*var;

	if (cmd_with_args[1])
		return (ft_printf_err("env only works with no args"), EXIT_FAILURE);
	env = msh->env;
	while (env)
	{
		var = env->content;
		ft_dprintf(fd_out, "%s=%s\n", var->name, var->value);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
