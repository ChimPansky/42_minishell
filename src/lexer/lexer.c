/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 23:34:19 by tkasbari         ###   ########.fr       */
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

static void	init_lexer(t_lexer *lexer, t_tokenlist **tokens, char *input)
{
	lexer->tokens = tokens;
	*lexer->tokens = NULL;
	lexer->last_tk_type = TK_NULL;
	lexer->pos_in_input = input;
	lexer->redir_type = FD_NULL;
}

int lex(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	t_lexer		lexer;

	init_lexer(&lexer, tokens_p, input);
	while (*lexer.pos_in_input)
	{
		if (ft_isspace(*lexer.pos_in_input))
			lexer.pos_in_input++;
		else if (*lexer.pos_in_input == '<' || *lexer.pos_in_input == '>')
		{
			if (read_tk_redir(msh, &lexer) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*lexer.pos_in_input == '&' || *lexer.pos_in_input == '|')
		{
			if (read_tk_simple_cmd_separator(msh, &lexer)	!= SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*lexer.pos_in_input == '(')
		{
			if (read_tk_subshell(msh, &lexer) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (read_tk_word(msh, &lexer)!= SUCCESS)
			return (tokenlist_destroy(tokens_p), !SUCCESS);
	}
	return (SUCCESS);
}
