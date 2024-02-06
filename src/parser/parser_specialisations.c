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
	if (redir->type == FD_HEREDOC)
		return (expand_heredoc(msh, &redir->content) != SUCCESS);
	return (expand_string_to_arr(msh, redir->string.buf, &redir->content));
}

int	parse_word(t_msh *msh, t_parser *parser)
{
	if (parser->cmd->cmd_type == CMD_SUBSHELL)
		return (ft_printf_err("CRIT ERR: got subshell in a regular comand"),
			!SUCCESS);
	if (parser->cmd->cmd_type == CMD_NULL
		&& SUCCESS != command_specialise(parser->cmd, CMD_EXEC))
		return (perror("parse_word: command_specialise"), !SUCCESS);
	return (expand_string_to_arr(msh, parser->token->string.buf,
			&parser->cmd->cmd_with_args));
}

int	parse_subshell(t_parser *parser)
{
	if (parser->cmd->cmd_type == CMD_SUBSHELL)
		return (ft_printf_err("CRIT ERR: got multiple subshells in one comand"),
			!SUCCESS);
	if (parser->cmd->cmd_type == CMD_EXEC)
		return (ft_printf_err("CRIT ERR: got subshell in a regular comand"),
			!SUCCESS);
	if (SUCCESS != command_specialise(parser->cmd, CMD_SUBSHELL))
		return (perror("parse_subshell: command_specialise"), !SUCCESS);
	parser->cmd->subcommand = parser->token->subshell;
	return (SUCCESS);
}

int	parse_separators(t_msh *msh, t_parser *parser, t_token_type tk_type)
{
	execute(msh, parser->cmdlist);
	cmdlist_destroy(&parser->cmdlist);
	parser->cmd = NULL;
	if (!parser->token
		|| (msh->last_exit_code == SUCCESS && tk_type == TK_LOGIC_OR)
		|| (msh->last_exit_code != SUCCESS && tk_type == TK_LOGIC_AND))
		parser->done = true;
	return (SUCCESS);
}
