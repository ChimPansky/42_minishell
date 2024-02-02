/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:40:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 14:41:13 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

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

static int  read_redir(t_redir_detail **redir, char **input)
{
	t_redir_detail	*new_redir;
	t_string		*read_target;

	new_redir = redir_create();
	if (!new_redir)
		return (perror("lex"), !SUCCESS);

	new_redir->whitespace_expansion = false;
	if (ft_strncmp("<<", *input, 2) == MATCH)
		new_redir->type = FD_HEREDOC;
	else if (ft_strncmp(">>", *input, 2) == MATCH)
		new_redir->type = FD_OUT_APPEND;
	else if (**input == '<')
		new_redir->type = FD_IN;
	else if (**input == '>')
		new_redir->type = FD_OUT_TRUNC;
	if (new_redir->type == FD_HEREDOC || new_redir->type == FD_OUT_APPEND)
		(*input) += 2;
	else
		(*input) += 1;
	if (redir->type == FD_HEREDOC)
		read_target = &new_redir->delimiter;
	else
		read_target = &new_redir->content;
	if (read_word(input, read_target) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	return (SUCCESS);
}
// turns input into token_list; stores token_list in msh.tokens
int lex(t_msh *msh, t_tokenlist **tokens_p, char *input)
{
	t_redir_detail	*redir;
	t_string		str;
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
