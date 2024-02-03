/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_variables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:35:04 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:35:17 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_variables.h"
#include "libft.h"
#include "ft_charptr_array.h"

t_var *variable(const char *name, const char *value)
{
	t_var		*var;
	const char	*default_value = "";

	var = malloc(sizeof(t_var));
	if (!var)
		return NULL;
	var->name = ft_strdup(name);
	if (!var->name)
		return (free(var), NULL);
	if (!value)
		value = ft_strdup(default_value);
	else
		var->value = ft_strdup(value);
	if (!var->value)
		return (free(var->name), free(var), NULL);
	return var;
}

void destroy_variable(void *var_void)
{
	t_var *var = var_void;

	free(var->name);
	free(var->value);
	free(var);
}

char	*varlist_get_value_by_substr(t_varlist *vars, const char *str, size_t sz)
{
	t_var	*target;

	while (vars)
	{
		target = vars->content;
		if (ft_strncmp(target->name, str, sz) == SUCCESS
				&& !target->name[sz])
			return target->value;
		vars = vars->next;
	}
	return ("");
}

char	*varlist_get_value(t_varlist *vars, const char *name)
{
	t_var	*target;

	target = varlist_find(vars, name);
	if (!target)
		return ("");
	return (target->value);
}

t_var *varlist_find(t_varlist *vars, const char *name)
{
	while (vars)
	{
		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
			return vars->content;
		vars = vars->next;
	}
	return NULL;
}

t_var *varlist_set(t_varlist **vars_p, const char *name, const char *value)
{
	t_var *var = variable(name, value);
	if (!var)
		return NULL;
	t_varlist *new_var = ft_lstnew(var);
	if (!new_var)
		return (destroy_variable(var), NULL);
	ft_lstadd_front(vars_p, new_var);
	return var;
}

void varlist_delete_one(t_varlist **vars_p, const char *name)
{
	t_varlist *vars;

	vars = *vars_p;
	while (vars)
	{
		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
		{
			ft_lstdel_node(vars_p, vars, destroy_variable);
			return ;
		}
		vars = vars->next;
	}
}

void varlist_destoy(t_varlist **vars_p)
{
	ft_lstclear(vars_p, destroy_variable);
}

// on fail returns NULL
t_varlist *varlist_init_from_envp(char **envp)
{
	t_varlist	*env;
	char	*sep;

	env = NULL;
	if (!envp)
		return NULL;
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		*sep = 0;
		if (NULL == varlist_set(&env, *envp, sep + 1))
			return (ft_lstclear(&env, destroy_variable), NULL);
		envp++;
	}
	return env;
}

// on fail returns NULL
int varlist_convert_to_array(t_varlist *vars, t_charptr_array *arr)
{
	char	*allocated;
	if (charptr_array_init(arr) != SUCCESS)
		return !SUCCESS;
	while (vars)
	{
		allocated = ft_strnjoin(3, ((t_var*)vars->content)->name, "=", ((t_var*)vars->content)->value);
		if (!allocated || charptr_array_add_allocated_str(arr, &allocated) != SUCCESS)
			return free(allocated), charptr_array_destroy(arr), !SUCCESS;
		vars = vars->next;
	}
	return SUCCESS;
}
