/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_specialisations.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:51:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/07 17:51:34 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	expand_var_in_glob(
	t_expander *expander,
	const char *var_content,
	t_charptr_array *arr)
{
	const char	*pos_in_var = var_content;

	while (*pos_in_var)
	{
		if (ft_isspace(*pos_in_var))
		{
			if (!string_is_empty(&expander->replace))
			{
				if (SUCCESS != expand_wildcard_and_finalize(expander, arr))
					return (!SUCCESS);
				if (SUCCESS != string_init(&expander->replace, ""))
					return (perror("expand_var_in_glob: "
							"string_init"), !SUCCESS);
				ft_lstclear(&expander->true_wildcards, free);
			}
			pos_in_var++;
		}
		else
		{
			if (SUCCESS != check_for_wc_and_improve(expander, &pos_in_var))
				return (!SUCCESS);
		}
	}
	return (SUCCESS);
}

int	expand_singleq(t_expander *expander)
{
	expander->pos++;
	while (*expander->pos && *expander->pos != '\'')
		if (SUCCESS != string_add_chr(&expander->replace, *expander->pos++))
			return (perror("expand_singleq: string_add_chr"), !SUCCESS);
	if (!*expander->pos)
		return (ft_printf_err("CRIT ERR: unexpected eol\n"), !SUCCESS);
	expander->pos++;
	return (SUCCESS);
}

int	expand_variable(t_msh *msh, t_expander *expander, t_charptr_array *arr)
{
	char	*var_content;

	expander->pos++;
	var_content = get_var_content(msh, &expander->pos);
	if (expander->glob)
		return (expand_var_in_glob(expander, var_content, arr));
	if (SUCCESS != string_add_str(&expander->replace, var_content))
		return (perror("expand_variable: string_add_chr"), !SUCCESS);
	return (SUCCESS);
}
