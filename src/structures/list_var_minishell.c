/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_var_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:35:04 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:24:10 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "list_variables.h"
#include "ft_charptr_array.h"

t_var	*variable(const char *name, const char *value);
void	destroy_variable(void *var_void);

char	*varlist_get_value_by_substr(
	t_varlist *vars, const char *str, size_t sz)
{
	t_var	*target;

	while (vars)
	{
		target = vars->content;
		if (ft_strncmp(target->name, str, sz) == SUCCESS
			&& !target->name[sz])
			return (target->value);
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

// on fail returns NULL
t_varlist	*varlist_init_from_envp(char **envp)
{
	t_varlist	*env;
	char		*sep;

	env = NULL;
	if (!envp)
		return (NULL);
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		*sep = 0;
		if (NULL == varlist_set(&env, *envp, sep + 1))
			return (ft_lstclear(&env, destroy_variable), NULL);
		envp++;
	}
	return (env);
}

int	varlist_convert_to_array(t_varlist *vars, t_charptr_array *arr)
{
	char	*allocated;

	if (charptr_array_init(arr) != SUCCESS)
		return (!SUCCESS);
	while (vars)
	{
		allocated = ft_strnjoin(3, ((t_var *)vars->content)->name, "=",
				((t_var*)vars->content)->value);
		if (!allocated
			|| charptr_array_add_allocated_str(arr, &allocated) != SUCCESS)
			return (free(allocated), charptr_array_destroy(arr), !SUCCESS);
		vars = vars->next;
	}
	return (SUCCESS);
}
