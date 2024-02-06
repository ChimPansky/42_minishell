/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/05 14:42:58 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static void	read_redir_type(t_lexer *lexer)
{
	if (ft_strncmp("<<", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->redir_type = FD_HEREDOC;
	}
	else 	if (ft_strncmp(">>", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->redir_type = FD_OUT_APPEND;
	}
	else if (*lexer->pos_in_input == '<')
	{
		lexer->pos_in_input += 1;
		lexer->redir_type = FD_IN;
	}
	else if (*lexer->pos_in_input == '>')
	{
		lexer->pos_in_input += 1;
		lexer->redir_type = FD_OUT_TRUNC;
	}
	else {
		lexer->redir_type = FD_NULL;
	}
}

int	read_tk_redir(t_msh *msh,  t_lexer *lexer)
{
	t_token			*new_token;
	t_redir_detail	*new_redir;

	new_token = tokenlist_add_token(lexer->tokens, TK_REDIR);
	if (!new_token)
		return (!SUCCESS);
	read_redir_type(lexer);
	if (redir_init(&new_redir, lexer->redir_type) != SUCCESS)
		return (!SUCCESS);
	if (read_word(msh, lexer, &(new_redir->string)) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	new_token->redir = new_redir;
	lexer->last_tk_type = TK_REDIR;
	return (SUCCESS);
}
