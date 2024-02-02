#include "minishell.h"

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


size_t read_shell_spaces(char **input)
{
	size_t  sep_count;

	sep_count = 0;
	while (is_shell_space(**input))
	{
		*(input) += 1;
		sep_count++;
	}
	return (sep_count);
}

int read_word(char **input, t_string *str)
{
	char    quote_type;

	if (string_init(str, "") != SUCCESS)
		return (perror("lex: read word"), !SUCCESS);
	quote_type = 0;
	read_shell_spaces(input);
	while (**input)
	{
		if (quote_type == 0 && is_token_seperator(**input))
			break;
		if (quote_type == 0 && (**input == '\'' || **input == '"')) // found opening quote
			quote_type = **input;
		else if (**input == quote_type)  // found closing quote
			quote_type = 0;
		string_add_chr(str, **input);
		(*input) += 1;
	}
	if (quote_type != 0)
		return (string_destroy(str), !SUCCESS);
	return (SUCCESS);
}

static int  read_redir(t_redir_detail *redir, char **input)
{
	if (ft_strncmp("<<", *input, 2) == MATCH)
		redir->type = FD_HEREDOC;
	else if (ft_strncmp(">>", *input, 2) == MATCH)
		redir->type = FD_OUT_APPEND;
	else if (**input == '<')
		redir->type = FD_IN;
	else if (**input == '>')
		redir->type = FD_OUT_TRUNC;
	if (redir->type == FD_HEREDOC || redir->type == FD_OUT_APPEND)
		(*input) += 2;
	else
		(*input) += 1;
	if (read_word(input, &redir->string) != SUCCESS)
		return (!SUCCESS);
	return (SUCCESS);
}

// turns input into token_list; stores token_list in msh.tokens
int lex(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	t_redir_detail  redir;
	t_string        str;
	t_token_type	last_token_type;
	//char            *word;

	// grep NAME < Makefile > out1.txt |
	last_token_type = TK_NULL;
	*tokens_p = NULL;
	while (*input)
	{
		if (*input == '<' || *input == '>')
		{
			//handle_redir()...
			if (read_redir(&redir, &input) != SUCCESS)
			{
				msh->last_exit_code = ER_UNEXPECTED_TOKEN;
				error_unexpected_token("newline");
				return (tokenlist_destroy(tokens_p), !SUCCESS);
			}
			if (!token_add(tokens_p, TK_REDIR, &str, &redir))
				return (perror("lex"), tokenlist_destroy(tokens_p),  !SUCCESS);
			else
				last_token_type = TK_REDIR;
		}
		else if (*input == '|')
		{
			//handle_pipe()...
			if (last_token_type == TK_NULL || last_token_type == TK_PIPE)
			{
				msh->last_exit_code = ER_UNEXPECTED_TOKEN;
				error_unexpected_token("|");
				return (tokenlist_destroy(tokens_p),  !SUCCESS);
			}
			if (!token_add(tokens_p, TK_PIPE, &str, NULL))
				return (perror("lex"), tokenlist_destroy(tokens_p),  !SUCCESS);
			last_token_type = TK_PIPE;
			input++;
		}
		else if (is_shell_space(*input))
			input++;
		else
		{
			//handle_word()...
			if (read_word(&input, &str) != SUCCESS)
			{   // TODO: there was a problem reading the word...
				msh->last_exit_code = ER_UNEXPECTED_TOKEN;
				error_unexpected_token("newline");
				return (tokenlist_destroy(tokens_p),  !SUCCESS);
			}
			else
			{
				if (!token_add(tokens_p, TK_WORD, &str, NULL))
					return (perror("lex"), tokenlist_destroy(tokens_p),  !SUCCESS);
				last_token_type = TK_WORD;
			}
		}
	}
	if (last_token_type != TK_WORD)
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		error_unexpected_token("newline");
		return (tokenlist_destroy(tokens_p),  !SUCCESS);
	}
	return (SUCCESS);
}
