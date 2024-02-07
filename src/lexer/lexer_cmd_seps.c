/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd_seps.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 20:59:56 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static void	lex_tk_simple_cmd_separator_type(t_lexer *lexer)
{
	if (ft_strncmp("&&", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->cur_tk_type = TK_LOGIC_AND;
	}
	else if (ft_strncmp("||", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->cur_tk_type = TK_LOGIC_OR;
	}
	else if (*lexer->pos_in_input == '|')
	{
		lexer->pos_in_input += 1;
		lexer->cur_tk_type = TK_PIPE;
	}
	else {
		lexer->cur_tk_type = TK_NULL;
	}
}

int	lex_tk_simple_cmd_separator(t_msh *msh, t_lexer *lexer)
{
	lex_tk_simple_cmd_separator_type(lexer);

	if (check_unexpected_token(msh, lexer->last_tk_type,
		lexer->cur_tk_type, lexer) != SUCCESS)
		return (!SUCCESS);
	if (!tokenlist_add_token(lexer->tokens, lexer->cur_tk_type))
		return (!SUCCESS);
	read_shell_spaces(&lexer->pos_in_input);
	if (!*lexer->pos_in_input || *lexer->pos_in_input == '&' || *lexer->pos_in_input == '|')
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (error_unexp_tk_c(msh, *lexer->pos_in_input), !SUCCESS);
	}
	lexer->last_tk_type = lexer->cur_tk_type;
	return (SUCCESS);
}
