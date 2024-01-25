/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/25 10:55:13 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

// TODO
static int	print_env(t_msh *msh, int fd_out)
{
	(void) msh;
	(void) fd_out;
	return SUCCESS;
}

static int add_var(t_msh *msh, char* var_descr)
{
	char	*pos_equals;
	char	*var_value;

	pos_equals = ft_strchr(var_descr, '=');
	if (pos_equals)
	{
		*pos_equals = '\0';
		var_value = pos_equals + 1;
		if (!var_set(&msh->env, var_descr, var_value))
			return (!SUCCESS);
	}
	else
	{
		//find local variable with name == var_name and copy (export) it to env or if not exist, export with empty value
		var_value = "";
	}
	return SUCCESS;
}

int	built_in_export(t_msh *msh, char **cmd_with_args, int fd_out)
{
	if (!cmd_with_args || !*cmd_with_args++)
		return(ft_printf("this shouldnt happen"), !SUCCESS); // todo
	if (!*cmd_with_args)
		return (print_env(msh, fd_out));
	while (*cmd_with_args)
		if (add_var(msh, *cmd_with_args++) != SUCCESS)
			return (!SUCCESS);
	return (SUCCESS);
}
