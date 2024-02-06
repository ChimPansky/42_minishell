/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/05 14:55:37 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "ft_string.h"
#include "lexer.h"


int read_word(t_msh *msh, t_lexer *lexer, t_string *str)
{
	char    quote_type;

	quote_type = 0;
	while (*lexer->pos_in_input)
	{
		if (quote_type == 0 && is_token_seperator(*lexer->pos_in_input))
			break;
		if (quote_type == 0
			&& (*lexer->pos_in_input == '\'' || *lexer->pos_in_input == '"'))
			quote_type = *lexer->pos_in_input;
		else if (*lexer->pos_in_input == quote_type)
			quote_type = 0;
		string_add_chr(str, *lexer->pos_in_input);
		(lexer->pos_in_input) += 1;
	}
	if (quote_type != 0 || string_is_empty(str))
	{
		error_unexpected_token(*lexer->pos_in_input);
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (!SUCCESS);
	}
	return (SUCCESS);
}

int	read_tk_word(t_msh *msh, t_lexer *lexer)
{
	t_token			*new_token;

	new_token = tokenlist_add_token(lexer->tokens, TK_WORD);
	if (!new_token)
		return (!SUCCESS);
	if (read_word(msh, lexer, &new_token->string) != SUCCESS)
		return (!SUCCESS);
	lexer->last_tk_type = TK_WORD;
	return (SUCCESS);
}
