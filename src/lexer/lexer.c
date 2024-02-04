/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 10:37:42 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces


// LEXER (input) -> token list
//
// CHECK ERRS:
//  redirect token contains doc
//  bracket check (bracket inside quote case as well)
//  quotes check
//  pipe, end, or, ; at eol
//
// PARSER (token list) -> cmdlist
//      expander is part of parser
//      check for correct doc names as well and expand (heredoc delim not expandable)
//
// EXECUTOR

// turns input into token_list; stores token_list in msh.tokens
int lex(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	t_token_type	last_tk_type;
	char			*pos_in_input;

	pos_in_input = input;
	last_tk_type = TK_NULL;
	*tokens_p = NULL;
	while (*pos_in_input)
	{
		if (is_shell_space(*pos_in_input))
			pos_in_input++;
		else if (*pos_in_input == '<' || *pos_in_input == '>')
		{
			if (read_tk_redir(msh, tokens_p, &pos_in_input, &last_tk_type) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*pos_in_input == '&' || *pos_in_input == '|')
		{
			if (read_tk_and_or_pipe(msh, tokens_p, &pos_in_input, &last_tk_type)
				!= SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (read_tk_word(msh, tokens_p, &pos_in_input, &last_tk_type)!= SUCCESS)
			return (tokenlist_destroy(tokens_p), !SUCCESS);
		// TODO: read TK_SUBSHELL... '('
	}
	//print_tokens(*tokens_p);
	return (SUCCESS);
}
