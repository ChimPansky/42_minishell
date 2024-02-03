/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/03 16:11:53 by tkasbari         ###   ########.fr       */
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
	t_token_type	last_token_type;
	char			*pos_in_input;

	pos_in_input = input;
	last_token_type = TK_NULL;
	*tokens_p = NULL;
	while (*pos_in_input)
	{
		if (*pos_in_input == '<' || *pos_in_input == '>')
		{
			if (read_tk_redir(msh, tokens_p, &pos_in_input) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
			last_token_type = TK_REDIR;
		}
		else if (*pos_in_input == '|')
		{
			if (last_token_type == TK_NULL || last_token_type == TK_PIPE)
			{
				msh->last_exit_code = ER_UNEXPECTED_TOKEN;
				error_unexpected_token("|");
				return (tokenlist_destroy(tokens_p), !SUCCESS);
			}
			//if (!token_add(tokens_p, TK_PIPE, &str, NULL))
			//	return (perror("lex"), tokenlist_destroy(tokens_p),  !SUCCESS);
			last_token_type = TK_PIPE;
			pos_in_input++;
		}
		else if (is_shell_space(*pos_in_input))
			pos_in_input++;
		else
		{
			if (read_tk_word(msh, tokens_p, &pos_in_input) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
			last_token_type = TK_WORD;
		}
	}
	if (last_token_type == TK_PIPE)
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		error_unexpected_token("newline");
		return (tokenlist_destroy(tokens_p),  !SUCCESS);
	}
	return (SUCCESS);
}
