/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 14:42:51 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

char	*get_last_exit_code(t_msh *msh)
{
	static char	ret_code_buf[4];
	int			pos;

	pos = 0;
	if (msh->last_exit_code > 100)
		ret_code_buf[pos++] = '0' + msh->last_exit_code / 100 % 10;
	if (msh->last_exit_code > 10)
		ret_code_buf[pos++] = '0' + msh->last_exit_code / 10 % 10;
	ret_code_buf[pos++] = '0' + msh->last_exit_code % 10;
	ret_code_buf[pos] = '\0';
	return (ret_code_buf);
}

static char	*get_env_var_content(t_msh *msh, char **pos_in_string)
{
	int		name_len;
	char	*content;

	name_len = 0;
	while (!is_var_separator((*pos_in_string)[name_len]))
		name_len++;
	if (name_len == 0)
		return ("");
	content = varlist_get_value_by_substr(msh->env, *pos_in_string, name_len);
	*pos_in_string += name_len;
	return (content);
}

// if more spec variables
// is_special_var_name -> get_spec_var_value_by_name
char	*get_var_content(t_msh *msh, char **pos_in_string)
{
	if (**pos_in_string == '?')
	{
		*pos_in_string += 1;
		return (get_last_exit_code(msh));
	}
	return (get_env_var_content(msh, pos_in_string));
}
