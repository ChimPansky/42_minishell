/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 23:30:49 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

typedef struct s_lexer
{
	t_tokenlist		**tokens;
	t_token_type	last_tk_type;
	char			*pos_in_input;
	t_redir_type	redir_type;
}		t_lexer;

int		read_tk_redir(t_msh *msh,  t_lexer *lexer);
int		read_tk_word(t_msh *msh, t_lexer *lexer);
int		read_tk_simple_cmd_separator(t_msh *msh, t_lexer *lexer);
int		read_tk_subshell(t_msh *msh,  t_lexer *lexer);
size_t	read_shell_spaces(char **input);

#endif  // LEXER__H
