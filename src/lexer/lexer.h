/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 10:40:11 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

int		read_tk_redir(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
		t_token_type *last_tk_type);
int		read_tk_word(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
				t_token_type *last_tk_type);
int		read_tk_and_or_pipe(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
		t_token_type *last_tk_type);
size_t read_shell_spaces(char **input);



int		expand_string_to_arr(t_msh *msh, char *string,
		t_charptr_array *str_array);
int		expand_heredoc(t_msh *msh, t_charptr_array *heredoc_lines);
char	*get_last_exit_code(t_msh *msh);
char	*get_var_content(t_msh *msh, char **pos_in_str);



#endif  // LEXER__H
