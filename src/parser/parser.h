/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:33:12 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_PARSER__H
# define PARSER_PARSER__H

# include "../minishell.h"

int		expand_string_to_arr(t_msh *msh, char *string, t_charptr_array *str_array);
int		expand_heredoc(t_msh *msh, t_charptr_array *heredoc_lines);

char	*get_last_exit_code(t_msh *msh);
char	*get_var_content(t_msh *msh, char **pos_in_str);



#endif  // PARSER_PARSER__H
