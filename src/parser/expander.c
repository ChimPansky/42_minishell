/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 15:48:20 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

int	expander_init(t_expander *expander, char *string)
{
	expander->glob = true;
	expander->pos = string;
	if (SUCCESS != string_init(&expander->replace, ""))
		return (perror("expander_init: string_init"), !SUCCESS);
	return (SUCCESS);
}

void	expander_destroy(t_expander *expander)
{
	string_destroy(&expander->replace);
}

int	expand_next(t_msh *msh, t_expander *expander, t_charptr_array *arr)
{
	if (expander->glob && *expander->pos == '\'')
		return (expand_singleq(expander) != SUCCESS);
	if (*expander->pos == '"')
	{
		expander->glob = !expander->glob;
		expander->pos++;
		return (SUCCESS);
	}
	if (*expander->pos == '$' && is_var_name_start(expander->pos[1]))
		return (expand_variable(msh, expander, arr));
	if (SUCCESS != string_add_chr(&expander->replace, *expander->pos++))
		return (perror("expand_next: string_add_chr"),
			string_destroy(&expander->replace), !SUCCESS);
	return (SUCCESS);
}

int	expand_string_to_arr(t_msh *msh, char *string, t_charptr_array *arr)
{
	t_expander	expander;

	if (SUCCESS != expander_init(&expander, string))
		return (!SUCCESS);
	while (*expander.pos)
	{
		if (SUCCESS != expand_next(msh, &expander, arr))
			return (expander_destroy(&expander), !SUCCESS);
	}
	if (!expander.glob)
		return (ft_printf_err("CRIT ERR: unexpected eol"),
			expander_destroy(&expander), !SUCCESS);
	if (!string_is_empty(&expander.replace))
		if (SUCCESS
			!= charptr_array_add_allocated_str(arr, &expander.replace.buf))
			return (perror("expand_string_to_arr: "
					"charptr_array_add_allocated_str"),
				expander_destroy(&expander), !SUCCESS);
	return (expander_destroy(&expander), SUCCESS);
}
