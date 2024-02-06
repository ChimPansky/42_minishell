/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd_seps.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/05 14:49:25 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static void	read_tk_simple_cmd_separator_type(t_lexer *lexer)
{
	if (ft_strncmp("&&", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->last_tk_type = TK_LOGIC_AND;
	}
	else if (ft_strncmp("||", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->last_tk_type = TK_LOGIC_OR;
	}
	else if (*lexer->pos_in_input == '|')
	{
		lexer->pos_in_input += 1;
		lexer->last_tk_type = TK_PIPE;
	}
	else {
		lexer->last_tk_type = TK_NULL;
	}
}

int	read_tk_simple_cmd_separator(t_msh *msh, t_lexer *lexer)
{
	if (lexer->last_tk_type == TK_NULL)
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (error_unexpected_token(*lexer->pos_in_input), !SUCCESS);
	}
	read_tk_simple_cmd_separator_type(lexer);
	if (!tokenlist_add_token(lexer->tokens, lexer->last_tk_type))
		return (!SUCCESS);
	read_shell_spaces(&lexer->pos_in_input);
	if (!*lexer->pos_in_input || *lexer->pos_in_input == '&' || *lexer->pos_in_input == '|')
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (error_unexpected_token(*lexer->pos_in_input), !SUCCESS);
	}
	return (SUCCESS);
}
