/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 15:10:53 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static int is_valid_var_name(char *name)
{
	if (!(ft_isalpha(*name) || *name == '_'))
		return !SUCCESS;
	while (++name)
	{
		if (!(ft_isalnum(*name) || *name == '_'))
			return !SUCCESS;
	}
	return SUCCESS;

}

static int export_add_var(t_msh *msh, char* var_descr)
{
	char	*pos_equals;
	char	*var_value;

	pos_equals = ft_strchr(var_descr, '=');
	if (pos_equals)
	{
		*pos_equals = '\0';
		var_value = pos_equals + 1;
	}
	else
		var_value = "";
	if (!is_valid_var_name(var_descr))
		return (ft_printf_err("export: \'%s\': not a valid identifier\n",
			var_descr), !SUCCESS);
	if (!varlist_set(&msh->env, var_descr, var_value))
		return (perror("export"), !SUCCESS);
	return SUCCESS;
}

static int	export_output_args(t_msh *msh, int fd_out)
{
	t_varlist	*env;
	t_var		*var;

	env = msh->env;
	while (env)
	{
		var = env->content;
		ft_dprintf(fd_out, "export %s=\"%s\"\n", var->name, var->value);
		env = env->next;
	}
	return EXIT_SUCCESS;
}

int	built_in_export(t_msh *msh, char **cmd_with_args, int fd_out)
{
	cmd_with_args++;
	if (!*cmd_with_args)
		return (export_output_args(msh, fd_out));
	while (*cmd_with_args)
		if (export_add_var(msh, *cmd_with_args++) != SUCCESS)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
