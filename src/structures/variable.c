/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:35:04 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:21:54 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "list_variables.h"
#include "ft_charptr_array.h"

t_var	*variable(const char *name, const char *value)
{
	t_var		*var;
	const char	*default_value = "";

	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	if (!var->name)
		return (free(var), NULL);
	if (!value)
		value = ft_strdup(default_value);
	else
		var->value = ft_strdup(value);
	if (!var->value)
		return (free(var->name), free(var), NULL);
	return (var);
}

void	destroy_variable(void *var_void)
{
	t_var	*var;

	var = var_void;
	free(var->name);
	free(var->value);
	free(var);
}
