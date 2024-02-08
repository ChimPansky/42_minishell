/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 19:00:36 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"
#include "libft.h"
#include <unistd.h>


static void	find_end_of_sub_input(t_lexer *lexer)
{
	char	quote_type;
	int		open_brackets;

	quote_type = 0;
	open_brackets = 1;
	while (*lexer->end_of_sub_input)
	{
		if (quote_type == 0 && *lexer->end_of_sub_input == '(')
			open_brackets++;
		else if (quote_type == 0 && *lexer->end_of_sub_input == ')')
			open_brackets--;
		else if (quote_type == 0 && (*lexer->end_of_sub_input == '\''
			|| *lexer->end_of_sub_input == '"'))
			quote_type = *lexer->end_of_sub_input;
		else if (quote_type != 0 && *lexer->end_of_sub_input == quote_type)
			quote_type = 0;
		if (open_brackets == 0)
			return ;
		lexer->end_of_sub_input++;
	}
}

static int	get_sub_input(t_msh *msh, t_lexer *lexer)
{
	lexer->end_of_sub_input = lexer->pos_in_input;
	find_end_of_sub_input(lexer);
	if (!*lexer->end_of_sub_input)
		return (error_unexp_tk_s(msh, "\n"), !SUCCESS);
	lexer->sub_input = ft_substr(lexer->pos_in_input, 0,
		lexer->end_of_sub_input - lexer->pos_in_input);
	if (!lexer->sub_input)
		return (perror("lex: lex_tk_subshell: get_sub_input"), !SUCCESS);
	return (SUCCESS);
}

int		lex_tk_subshell(t_msh *msh, t_lexer *lexer)
{
	t_tokenlist	*sub_tokens;
	t_token		*new_token;

	if (check_unexpected_token(msh, lexer->last_tk_type,
		TK_SUBSHELL, lexer) != SUCCESS)
		return (!SUCCESS);
	lexer->pos_in_input += 1;
	if (get_sub_input(msh, lexer) != SUCCESS)
		return (!SUCCESS);
	if (str_is_empty(lexer->sub_input))
		return (error_unexp_tk_s(msh, ")"), free(lexer->sub_input), !SUCCESS);
	if (lex(msh, &sub_tokens, lexer->sub_input) != SUCCESS)
		return (free(lexer->sub_input), !SUCCESS);
	lexer->pos_in_input = lexer->end_of_sub_input + 1;
	new_token = tokenlist_add_token(lexer->tokens, TK_SUBSHELL);
	if (!new_token)
		return (free(lexer->sub_input), tokenlist_destroy(&sub_tokens), !SUCCESS);
	new_token->subshell_tokens = sub_tokens;
	lexer->last_tk_type = TK_SUBSHELL;
	free(lexer->sub_input);
	return (SUCCESS);
}
