/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 20:27:00 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static bool	is_valid_var_name(char *name, size_t len)
{
	size_t	idx;

	if (!*name || !(ft_isalpha(*name) || *name == '_'))
		return (false);
	idx = 1;
	while (idx < len && name[idx])
	{
		if (!(ft_isalnum(name[idx]) || name[idx] == '_'))
			return (false);
		idx++;
	}
	return (true);
}

static int	export_add_var(t_msh *msh, char *var_descr)
{
	char	*pos_equals;
	char	*var_value;
	size_t	name_len;

	pos_equals = ft_strchr(var_descr, '=');
	if (!pos_equals)
		name_len = ft_strlen(var_descr);
	else
		name_len = pos_equals - var_descr;
	if (!is_valid_var_name(var_descr, name_len))
		return (ft_printf_err("export: \'%s\': not a valid identifier\n",
				var_descr), !SUCCESS);
	if (!pos_equals)
		return (SUCCESS);
	*pos_equals = '\0';
	var_value = pos_equals + 1;
	if (!varlist_set(&msh->env, var_descr, var_value))
		return (perror("export_add_var: varlist_set"), !SUCCESS);
	return (SUCCESS);
}

static int	export_output_args(t_msh *msh, int fd_out)
{
	t_varlist	*env;
	t_var		*var;
	char		*it;

	env = msh->env;
	while (env)
	{
		var = env->content;
		ft_dprintf(fd_out, "export %s=\"", var->name);
		it = var->value;
		while (*it)
		{
			if (*it == '"')
				write(fd_out, "\\\"", 2);
			else if (*it == '$')
				write(fd_out, "\\$", 2);
			else
				write(fd_out, it, 1);
			it++;
		}
		write(fd_out, "\"\n", 2);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

int	built_in_export(t_msh *msh, char **cmd_with_args, int fd_out)
{
	int	ret;

	ret = EXIT_SUCCESS;
	cmd_with_args++;
	if (!*cmd_with_args)
		return (export_output_args(msh, fd_out));
	while (*cmd_with_args)
	{
		if (export_add_var(msh, *cmd_with_args) != SUCCESS)
			ret = EXIT_FAILURE;
		cmd_with_args++;
	}
	return (ret);
}
