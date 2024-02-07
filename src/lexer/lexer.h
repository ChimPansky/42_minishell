/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 18:15:49 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"
# include "ft_string.h"
# include <stdbool.h>

typedef struct s_lexer
{
	t_tokenlist		**tokens;
	t_token_type	cur_tk_type;
	t_token_type	last_tk_type;
	t_redir_type	redir_type;
	char			*pos_in_input;
	char			*end_of_sub_input;
	char			*sub_input;
}		t_lexer;

// lexer.c
size_t	read_shell_spaces(char **input);
int 	lex_tokens(t_msh *msh, t_tokenlist **tokens_p, char *input);
int		check_unexpected_token(t_msh *msh, t_token_type last_tk_tyke,
		t_token_type cur_tk_type, t_lexer *lexer);

// lexer_redir.c
int		lex_tk_redir(t_msh *msh,  t_lexer *lexer);

// lexer_word.c
int		lex_tk_word(t_msh *msh, t_lexer *lexer);

// lexer_cmd_seps.c
int		lex_tk_simple_cmd_separator(t_msh *msh, t_lexer *lexer);

// lexer_subshell.c
int		lex_tk_subshell(t_msh *msh,  t_lexer *lexer);

// lexer_word.c
int 	read_word(t_msh *msh, t_lexer *lexer, t_string *str);

#endif  // LEXER__H
