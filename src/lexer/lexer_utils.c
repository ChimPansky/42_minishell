/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/03 15:54:23 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

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

int read_word(t_msh *msh, char **pos_in_input, t_string *str)
{
	char    quote_type;

	quote_type = 0;
	read_shell_spaces(pos_in_input);
	while (**pos_in_input)
	{
		if (quote_type == 0 && is_token_seperator(**pos_in_input))
			break;
		if (quote_type == 0
			&& (**pos_in_input == '\'' || **pos_in_input == '"'))
			quote_type = **pos_in_input;
		else if (**pos_in_input == quote_type)
			quote_type = 0;
		string_add_chr(str, **pos_in_input);
		(*pos_in_input) += 1;
	}
	read_shell_spaces(pos_in_input);
	if (quote_type != 0 || string_is_empty(str))
	{
		if (**pos_in_input == '\0')
			error_unexpected_token("newline");
		else
			error_unexpected_token(*pos_in_input);
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (!SUCCESS);
	}
	return (SUCCESS);
}

static	t_redir_type	read_redir_type(char **pos_in_input)
{
	if (ft_strncmp("<<", *pos_in_input, 2) == MATCH)
	{
		*pos_in_input += 2;
		return (FD_HEREDOC);
	}
	else 	if (ft_strncmp(">>", *pos_in_input, 2) == MATCH)
	{
		*pos_in_input += 2;
		return (FD_OUT_APPEND);
	}
	else if (**pos_in_input == '<')
	{
		*pos_in_input += 1;
		return (FD_IN);
	}
	else if (**pos_in_input == '>')
	{
		*pos_in_input += 1;
		return (FD_OUT_APPEND);
	}
	return (FD_NULL);
}

int  read_tk_redir(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input)
{
	t_token			*new_token;
	t_redir_detail	*new_redir;

	new_token = tokenlist_add_token(tokens_p, TK_REDIR);
	if (!new_token)
		return (!SUCCESS);
	if (redir_init(&new_redir, read_redir_type(pos_in_input)) != SUCCESS)
		return (!SUCCESS);
	if (read_word(msh, pos_in_input, &new_redir->string) != SUCCESS)
		return (redir_destroy(&new_redir), !SUCCESS);
	new_token->redir = new_redir;

	return (SUCCESS);
}

int  read_tk_word(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input)
{
	t_token			*new_token;

	new_token = tokenlist_add_token(tokens_p, TK_WORD);
	if (!new_token)
		return (!SUCCESS);
	if (read_word(msh, pos_in_input, &new_token->string) != SUCCESS)
		return (!SUCCESS);
	return (SUCCESS);
}

// int  read_redir(t_redir_detail *redir, char **input)
// {
// 	if (ft_strncmp("<<", *input, 2) == MATCH)
// 		redir->type = FD_HEREDOC;
// 	else if (ft_strncmp(">>", *input, 2) == MATCH)
// 		redir->type = FD_OUT_APPEND;
// 	else if (**input == '<')
// 		redir->type = FD_IN;
// 	else if (**input == '>')
// 		redir->type = FD_OUT_TRUNC;
// 	if (redir->type == FD_HEREDOC || redir->type == FD_OUT_APPEND)
// 		(*input) += 2;
// 	else
// 		(*input) += 1;
// 	if (read_word(input, &redir->string) != SUCCESS)
// 		return (!SUCCESS);
// 	return (SUCCESS);
// }
