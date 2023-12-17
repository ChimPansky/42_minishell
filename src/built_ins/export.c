/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 14:57:57 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_export(t_msh *msh, char **cmd_with_args)
{
	int	i;
	char	*var_name;
	char	*pos_equals;
	char	*var_value;

	i = 1;
	while (cmd_with_args[i])
	{
		var_name = cmd_with_args[i];
		pos_equals = ft_strchr(cmd_with_args[i], '=');
		if (pos_equals)
		{
			*pos_equals = '\0';
			var_value = pos_equals + 1;
			if (!var_set(&msh->env, var_name, var_value))
				return (1);
		}
		else
		{
			//find local variable with name == var_name and copy (export) it to env
			var_value = "";
		}
		i++;
	}
	return (SUCCESS);
}
