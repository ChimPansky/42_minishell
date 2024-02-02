/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 14:32:15 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

int expand_var_in_glob(char *var_content, t_string *replace, t_charptr_array *arr)
{
	char	*pos_in_var;

	pos_in_var = var_content;
	while (*pos_in_var)
	{
		if (ft_isspace(*pos_in_var))
		{
			if (!string_is_empty(replace))
			{
				if (SUCCESS != charptr_array_add_allocated_str(arr, &replace->buf))
					return !SUCCESS;
				if (SUCCESS != string_init(replace, ""))
					return !SUCCESS;
			}
		}
		else
		{
			if (SUCCESS != string_add_chr(replace, *pos_in_var))
				return !SUCCESS;
		}
		pos_in_var++;
	}
	return SUCCESS;
}

int expand_string_to_arr(t_msh *msh, char *string, t_charptr_array *arr)
{
	char		*pos_in_string;
	bool		glob;
	t_string	replace;

	if (SUCCESS != string_init(&replace, ""))
		return !SUCCESS;
	glob = true;
	pos_in_string = string;
	while (*pos_in_string)
	{
		if (glob && *pos_in_string == '\'')
		{
			pos_in_string++;
			while (*pos_in_string != '\'')
				if (SUCCESS != string_add_chr(&replace, *pos_in_string++))
					return (string_destroy(&replace), !SUCCESS);
			pos_in_string++;
		}
		else if (*pos_in_string == '"')
		{
			glob = !glob;
			pos_in_string++;
		}
		else if (*pos_in_string == '$' && !ft_isspace(pos_in_string[1]))
		{
			pos_in_string++;
			char *var_content = get_var_content(msh, &pos_in_string);
			if (!glob)
			{
				if (SUCCESS != string_add_str(&replace, var_content))
					return string_destroy(&replace), !SUCCESS;
			}
			else
			{
				if (SUCCESS != expand_var_in_glob(var_content, &replace, arr))
					return string_destroy(&replace), !SUCCESS;
			}
		}
		else
		{
			if (SUCCESS != string_add_chr(&replace, *pos_in_string++))
				return string_destroy(&replace), !SUCCESS;
		}
	}
	if (!string_is_empty(&replace))
		if (SUCCESS != charptr_array_add_allocated_str(arr, &replace.buf))
			return string_destroy(&replace), !SUCCESS;
	return SUCCESS;
}
