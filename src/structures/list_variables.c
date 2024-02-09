/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_variables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:35:04 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:23:37 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "list_variables.h"
#include "ft_charptr_array.h"

t_var	*variable(const char *name, const char *value);
void	destroy_variable(void *var_void);

t_var	*varlist_find(t_varlist *vars, const char *name)
{
	while (vars)
	{
		if (ft_strcmp(((t_var *)vars->content)->name, name) == SUCCESS)
			return (vars->content);
		vars = vars->next;
	}
	return (NULL);
}

t_var	*varlist_set(t_varlist **vars_p, const char *name, const char *value)
{
	t_var		*existing_var;
	t_var		*new_var;
	t_varlist	*new_varlist;

	existing_var = varlist_find(*vars_p, name);
	if (!existing_var)
	{
		new_var = variable(name, value);
		new_varlist = ft_lstnew(new_var);
		if (!new_varlist)
			return (destroy_variable(new_var), NULL);
		ft_lstadd_front(vars_p, new_varlist);
		return (new_var);
	}
	else
	{
		free(existing_var->value);
		existing_var->value = ft_strdup(value);
		if (!existing_var->value)
			return (NULL);
		return (existing_var);
	}
}

void	varlist_delete_one(t_varlist **vars_p, const char *name)
{
	t_varlist	*vars;

	vars = *vars_p;
	while (vars)
	{
		if (ft_strcmp(((t_var *)vars->content)->name, name) == SUCCESS)
		{
			ft_lstdel_node(vars_p, vars, destroy_variable);
			return ;
		}
		vars = vars->next;
	}
}

void	varlist_destoy(t_varlist **vars_p)
{
	ft_lstclear(vars_p, destroy_variable);
}
