/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_specialisations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:36:51 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/08 19:36:52 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// TODO change ft_list to redirlist
int	parse_redirection(t_msh *msh, t_parser *parser)
{
	t_redirlist		*new_redir;
	t_redirlist		**redirlist;
	t_redir_detail	*redir;

	redir = parser->token->redir;
	redirlist = &parser->cmd->redirections;
	new_redir = ft_lstnew(redir);
	if (!new_redir)
		return (perror("parse_redirection: ft_lstnew"), !SUCCESS);
	ft_lstadd_back(redirlist, new_redir);
	if (redir->type == FD_HEREDOC && redir->expand_heredoc)
		return (expand_heredoc(msh, &redir->content) != SUCCESS);
	return (expand_string_to_arr(msh, redir->string.buf, &redir->content));
}

int	parse_word(t_msh *msh, t_parser *parser)
{
	if (parser->cmd->cmd_type == CMD_SUBSHELL)
		return (ft_printf_err("CRIT ERR: "
				"got subshell_tokens in a regular comand\n"), !SUCCESS);
	if (parser->cmd->cmd_type == CMD_NULL
		&& SUCCESS != command_specialise(parser->cmd, CMD_EXEC))
		return (perror("parse_word: command_specialise"), !SUCCESS);
	return (expand_string_to_arr(msh, parser->token->string.buf,
			&parser->cmd->cmd_with_args));
}

int	parse_subshell(t_parser *parser)
{
	if (parser->cmd->cmd_type == CMD_SUBSHELL)
		return (ft_printf_err("CRIT ERR: "
				"got multiple subshells in one comand\n"), !SUCCESS);
	if (parser->cmd->cmd_type == CMD_EXEC)
		return (ft_printf_err("CRIT ERR: "
				"got subshell_tokens in a regular comand\n"), !SUCCESS);
	if (SUCCESS != command_specialise(parser->cmd, CMD_SUBSHELL))
		return (perror("parse_subshell: command_specialise"), !SUCCESS);
	parser->cmd->subcommand = parser->token->subshell_tokens;
	return (SUCCESS);
}

int	skip_to_next_sep(t_parser *parser)
{
	while (parser->tokenlist->next)
	{
		parser->tokenlist = parser->tokenlist->next;
		parser->token = parser->tokenlist->content;
		if (!parser->token)
			return (ft_printf_err("CRIT ERR: empty token\n"), !SUCCESS);
		parser->tk_type = parser->token->tk_type;
		if (is_cmd_separator_token(parser->tk_type))
			return (SUCCESS);
	}
	parser->tokenlist = NULL;
	parser->token = NULL;
	parser->tk_type = TK_NULL;
	return (SUCCESS);
}

int	parse_separators(t_msh *msh, t_parser *parser)
{
	int	ret_code;

	execute(msh, parser->cmdlist);
	cmdlist_destroy(&parser->cmdlist);
	parser->cmd = NULL;
	ret_code = msh->last_exit_code;
	while ((parser->tk_type == TK_LOGIC_OR && ret_code == EXIT_SUCCESS)
		|| (parser->tk_type == TK_LOGIC_AND && ret_code != EXIT_SUCCESS))
	{
		if (skip_to_next_sep(parser) != SUCCESS)
			return (!SUCCESS);
	}
	if (!parser->tokenlist)
		parser->done = true;
	return (SUCCESS);
}
