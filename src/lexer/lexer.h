/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 18:09:08 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

int		read_tk_redir(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
		t_token_type *last_tk_type);
int		read_tk_word(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
				t_token_type *last_tk_type);
int		read_tk_simple_cmd_separator(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
		t_token_type *last_tk_type);
size_t read_shell_spaces(char **input);

#endif  // LEXER__H
