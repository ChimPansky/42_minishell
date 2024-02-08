/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/06 13:21:41 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

static void	init_parser(t_parser *parser, t_tokenlist *tokenlist)
{
	parser->cmdlist = NULL;
	parser->cmd = NULL;
	parser->token = NULL;
	parser->done = false;
	parser->tokenlist = tokenlist;
}

static void	destroy_parser(t_parser *parser)
{
	cmdlist_destroy(&parser->cmdlist);
}

static int	init_token(t_parser *parser)
{
	if (!parser->tokenlist)
	{
		parser->token = NULL;
		parser->tk_type = TK_NULL;
		return (SUCCESS);
	}
	parser->token = parser->tokenlist->content;
	if (!parser->token)
		return (ft_printf_err("CRIT ERR: empty token content\n"), !SUCCESS);
	parser->tk_type = parser->token->tk_type;
	if (!parser->cmd)
		parser->cmd = cmdlist_add_cmd(&parser->cmdlist);
	if (!parser->cmd)
		return (perror("init_token: cmdlist_add_cmd"), !SUCCESS);
	return (SUCCESS);
}

static int	parse_token(t_msh *msh, t_parser *parser)
{
	const t_token_type	tk_type = parser->tk_type;

	if (is_cmd_separator_token(tk_type))
		return (parse_separators(msh, parser));
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

// msh->done may be true if exit was executed, protection from exit 1 && exit 2
int	parse_and_execute(t_msh *msh, t_tokenlist *tokenlist)
{
	t_parser			parser;

	if (!tokenlist)
		return (SUCCESS);
	init_parser(&parser, tokenlist);
	while (!parser.done && !msh->done && !g_sigint_received)
	{
		if (SUCCESS != init_token(&parser))
			return (destroy_parser(&parser), !SUCCESS);
		if (parse_token(msh, &parser) != SUCCESS)
			return (destroy_parser(&parser), !SUCCESS);
		if (!parser.done)
			parser.tokenlist = parser.tokenlist->next;
	}
	destroy_parser(&parser);
	return (SUCCESS);
}
