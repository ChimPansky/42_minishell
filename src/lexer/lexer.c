/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 18:41:24 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"
#include <stdbool.h>
#include "lexer.h"

// if brace, look for the other brace, execute in subshell_tokens
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

size_t read_shell_spaces(char **input)
{
	size_t  sep_count;

	sep_count = 0;
	while (**input && ft_isspace(**input))
	{
		*(input) += 1;
		sep_count++;
	}
	return (sep_count);
}

static void	init_lexer(t_lexer *lexer, t_tokenlist **tokens, char *input)
{
	lexer->tokens = tokens;
	*lexer->tokens = NULL;
	lexer->cur_tk_type = TK_NULL;
	lexer->last_tk_type = TK_NULL;
	lexer->redir_type = FD_NULL;
	lexer->pos_in_input = input;
	lexer->end_of_sub_input = NULL;
	lexer->sub_input = NULL;
}

int lex_tokens(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	t_lexer		lexer;

	init_lexer(&lexer, tokens_p, input);
	while (*lexer.pos_in_input)
	{
		read_shell_spaces(&lexer.pos_in_input);
		if (*lexer.pos_in_input == '<' || *lexer.pos_in_input == '>')
		{
			if (lex_tk_redir(msh, &lexer) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*lexer.pos_in_input == '&' || *lexer.pos_in_input == '|')
		{
			if (lex_tk_simple_cmd_separator(msh, &lexer)	!= SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*lexer.pos_in_input == '(')
		{
			if (lex_tk_subshell(msh, &lexer) != SUCCESS)
				return (tokenlist_destroy(tokens_p), !SUCCESS);
		}
		else if (*lexer.pos_in_input && lex_tk_word(msh, &lexer)!= SUCCESS)
			return (tokenlist_destroy(tokens_p), !SUCCESS);
	}
	return (SUCCESS);
}

int	check_unexpected_token(t_msh *msh, t_token_type last_type,
	t_token_type cur_type)
{
	char	*error_text;

	error_text = NULL;
	if (cur_type == TK_SUBSHELL && (last_type == TK_SUBSHELL
		|| last_type == TK_WORD || last_type == TK_REDIR))
		error_text = "(";
	else if (cur_type == TK_LOGIC_AND && (last_type == TK_LOGIC_AND
		|| last_type == TK_LOGIC_OR || last_type == TK_PIPE))
		error_text = "&";
	else if ((cur_type == TK_LOGIC_OR || cur_type == TK_PIPE)
		&& (last_type == TK_LOGIC_AND || last_type == TK_LOGIC_OR
		|| last_type == TK_PIPE))
		error_text = "|";
	if (error_text)
		return (error_unexp_tk_s(msh, error_text), !SUCCESS);
	return (SUCCESS);
}

int lex(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	return (lex_tokens(msh, tokens_p, input));
	// iterate throuh tokens and check if tokens are in a valid order...
	// for example: between to subshells there has to be a command separator
	// return (SUCCESS);
}
