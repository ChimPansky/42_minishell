/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 14:37:19 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

int 	lex(t_msh *msh, t_tokenlist **tokens_p, char *input);
size_t read_shell_spaces(char **input);
int		string_expand(t_msh *msh, t_string *str, bool *ws_expansion);
int		read_var_name(char **to_expand, t_string *var_name);

#endif  // LEXER_H
