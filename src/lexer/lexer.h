/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/05 17:35:49 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"
# include "ft_string.h"

typedef struct s_lexer
{
	t_tokenlist		**tokens;
	t_token_type	last_tk_type;
	t_redir_type	redir_type;
	char			*pos_in_input;
	char			*start_of_sub_input;
	char			*end_of_sub_input;
	char			*sub_input;
}		t_lexer;

// lexer.c
size_t	read_shell_spaces(char **input);

// lexer_redir.c
int		read_tk_redir(t_msh *msh,  t_lexer *lexer);

// lexer_word.c
int		read_tk_word(t_msh *msh, t_lexer *lexer);

// lexer_cmd_seps.c
int		read_tk_simple_cmd_separator(t_msh *msh, t_lexer *lexer);

// lexer_subshell.c
int		read_tk_subshell(t_msh *msh,  t_lexer *lexer);

// lexer_word.c
int 	read_word(t_msh *msh, t_lexer *lexer, t_string *str);

#endif  // LEXER__H
