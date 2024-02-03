/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:32:45 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

static void	init_parser(t_parser *parser)
{
	parser->cmdlist = NULL;
	parser->cmd = NULL;
	parser->token = NULL;
	parser->done = false;
}

static void	destroy_parser(t_parser *parser)
{
	cmdlist_destroy(&parser->cmdlist);
}

static int	init_token(t_parser *parser, t_tokenlist *tokenlist)
{
	if (!tokenlist)
	{
		parser->token = NULL;
		return (SUCCESS);
	}
	parser->token = tokenlist->content;
	if (!parser->token)
		return (ft_printf_err("CRIT ERR: empty token content"), !SUCCESS);
	if (!parser->cmd)
		parser->cmd = cmdlist_add_cmd(&parser->cmdlist);
	if (!parser->cmd)
		return (perror("init_token: cmdlist_add_cmd"), !SUCCESS);
	return (SUCCESS);
}

static int	parse_token(t_msh *msh, t_parser *parser)
{
	t_token_type	tk_type;

	if (parser->token)
		tk_type = parser->token->tk_type;
	else
		tk_type = TK_NULL;
	if (!parser->token || tk_type == TK_LOGIC_AND || tk_type == TK_LOGIC_OR)
		return (parse_separators(msh, parser, tk_type));
	else if (tk_type == TK_PIPE)
	{
		parser->cmd = NULL;
		return (SUCCESS);
	}
	else if (tk_type == TK_REDIR)
		return (parse_redirection(msh, parser));
	else if (tk_type == TK_WORD)
		return (parse_word(msh, parser));
	else if (tk_type == TK_SUBSHELL)
		return (parse_subshell(parser));
	return (ft_printf_err("CRIT ERR: unexpected token type: %d\n", tk_type),
		!SUCCESS);
}

int	parse_and_ecexute(t_msh *msh, t_tokenlist *tokenlist)
{
	t_parser			parser;

	init_parser(&parser);
	while (!parser.done)
	{
		if (SUCCESS != init_token(&parser, tokenlist))
			return (destroy_parser(&parser), !SUCCESS);
		if (parse_token(msh, &parser) != SUCCESS)
			return (destroy_parser(&parser), !SUCCESS);
		if (!parser.done)
			tokenlist = tokenlist->next;
	}
	destroy_parser(&parser);
	return (SUCCESS);
}
