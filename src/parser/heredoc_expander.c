/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/03 19:28:22 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

static int	expand_line(t_msh *msh, char **line)
{
	t_string	expanded;
	char		*pos_in_line;
	char		*var_content;

	pos_in_line = *line;
	if (string_init(&expanded, "") != SUCCESS)
		return (!SUCCESS);
	while (*pos_in_line)
	{
		if (*pos_in_line == '$' && !is_shell_space(pos_in_line[1]))
		{
			pos_in_line++;
			var_content = get_var_content(msh, &pos_in_line);
			if (string_add_str(&expanded, var_content) != SUCCESS)
				return (string_destroy(&expanded), !SUCCESS);
		}
		else
			if (string_add_chr(&expanded, *pos_in_line++) != SUCCESS)
				return (string_destroy(&expanded), !SUCCESS);
	}
	free(*line);
	*line = expanded.buf;
	return (SUCCESS);
}

int expand_heredoc(t_msh *msh, t_charptr_array *heredoc_lines)
{
	size_t	i;

	i = 0;
	while (i < heredoc_lines->sz)
	{
		if (heredoc_lines->buf[i][0] != '\0')
		{
			if (expand_line(msh, &heredoc_lines->buf[i]) != SUCCESS)
				return (!SUCCESS);
		}
		i++;
	}
	return (SUCCESS);
}
