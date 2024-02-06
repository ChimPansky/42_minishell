/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/05 17:57:47 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"
#include "libft.h"
#include <unistd.h>


static int	find_end_of_sub_input(t_lexer *lexer)
{
	static char	quote_type = 0;
	static int	open_brackets = 1;

	while (*lexer->pos_in_input)
	{
		if (quote_type == 0 && *lexer->pos_in_input == '(')
			open_brackets++;
		else if (quote_type == 0 && *lexer->pos_in_input == ')')
			open_brackets--;
		else if (quote_type == 0 && (*lexer->pos_in_input == '\''
			|| *lexer->pos_in_input == '"'))
			quote_type = *lexer->pos_in_input;
		else if (quote_type != 0 && *lexer->pos_in_input == quote_type)
			quote_type = 0;
		if (open_brackets == 0)
		{
			lexer->end_of_sub_input = lexer->pos_in_input;
			return (SUCCESS);
		}
		lexer->pos_in_input++;
	}
	return (SUCCESS);
}

static int	get_sub_input(t_msh *msh, t_lexer *lexer)
{
	lexer->start_of_sub_input = lexer->pos_in_input;
	lexer->end_of_sub_input = NULL;
	while (lexer->pos_in_input && find_end_of_sub_input(lexer) != SUCCESS)
		;
	if (!lexer->end_of_sub_input)
	{
		error_unexpected_token(*lexer->pos_in_input);
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (!SUCCESS);
	}
	lexer->sub_input = ft_substr(lexer->start_of_sub_input, 0,
		lexer->end_of_sub_input - lexer->start_of_sub_input);
	if (!lexer->sub_input)
		return (perror("lex: read_tk_subshell: get_sub_input"), !SUCCESS);
	return (SUCCESS);
}

int		read_tk_subshell(t_msh *msh, t_lexer *lexer)
{
	t_tokenlist	*tokens_sub;
	t_token		*new_token;

	lexer->pos_in_input += 1;
	if (get_sub_input(msh, lexer) != SUCCESS)
		return (!SUCCESS);
	ft_putstr_fd("subinput: ", STDOUT_FILENO);
	ft_putendl_fd(lexer->sub_input, STDOUT_FILENO);
	lex(msh, &tokens_sub, lexer->sub_input);
	new_token = tokenlist_add_token(lexer->tokens, TK_SUBSHELL);
	if (!new_token)
		return (free(lexer->sub_input), tokenlist_destroy(&tokens_sub), !SUCCESS);
	new_token->subshell = tokens_sub;
	//lexer->pos_in_input += 1;
	lexer->last_tk_type = TK_SUBSHELL;
	free(lexer->sub_input);
	return (SUCCESS);
}
