/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 11:49:41 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"

int deal_with_redir(t_msh *msh, t_redirlist **redirlist, t_redir_detail *redir)
{
	t_redirlist *new_redir;

	new_redir = ft_lstnew(redir); // add to redirlist func instead, redirlist will be destroyed if !SUCCESS
	if (!new_redir)
		return perror("parse: "), !SUCCESS;
	if (redir->type == FD_HEREDOC)
	{
		if (expand_heredoc(msh, &redir->content) != SUCCESS)
			return perror("parse: "), !SUCCESS;
		//printf("expanded heredoc:\n");
		//charptr_array_print(&redir->content);
	}
	else
	{
		if (expand_string_to_arr(msh, redir->string.buf, &redir->content) != SUCCESS)
			return perror("parse: "), !SUCCESS;
	}
	ft_lstadd_back(redirlist, new_redir);
	return SUCCESS;
}

// takes list of tokens and turns it into list of one or several commands (command chain); Also: perform variable expansions...
int 	parse_and_execute(t_msh *msh, t_tokenlist *tokens)
{
	t_cmdlist			*cmdlist;
	t_simple_command	*cmd;
	t_token				*token;
	t_redirlist			*redirs; // TODO: deal_with_redir() will add redir_detail to this list instead of cmd->redirlist. (its possible to have redirections without cmd...) everytime before a cmd should be executed: cmd->redirections = redirs...

	cmdlist = NULL;
	cmd = NULL;
	redirs = NULL;
	while (1)
	{
		if (tokens)
		{
			token = tokens->content;
			if (!token)  // check!
				exit(EXIT_FAILURE);
		}
		if (!tokens || token->tk_type == TK_LOGIC_AND || token->tk_type == TK_LOGIC_OR)
		{
			execute(msh, cmdlist);
			cmdlist_destroy(&cmdlist);
			cmd = NULL;
			if (!tokens
					|| (msh->last_exit_code == SUCCESS && token->tk_type == TK_LOGIC_OR)
					|| (msh->last_exit_code != SUCCESS && token->tk_type == TK_LOGIC_AND))
				break;
		}
		else if (token->tk_type == TK_PIPE)
			cmd = NULL;
		else if (token->tk_type == TK_WORD)
		{
			if (!cmd)
				cmd = cmdlist_add_cmd(&cmdlist, CMD_EXEC);
			if (!cmd)
				return (cmdlist_destroy(&cmdlist), !SUCCESS);
			if (cmd->cmd_type != CMD_EXEC)
				exit(EXIT_FAILURE); // syntax error
			if (expand_string_to_arr(msh, token->string.buf, &cmd->cmd_with_args) != SUCCESS)
				return (cmdlist_destroy(&cmdlist), !SUCCESS);
		}
		else if (token->tk_type == TK_REDIR)
		{
			if (deal_with_redir(msh, &cmd->redirections, token->redir) != SUCCESS)
				return (cmdlist_destroy(&cmdlist), !SUCCESS);
		}
		else if (token->tk_type == TK_SUBSHELL)
		{
			if (!cmd)
				cmd = cmdlist_add_cmd(&cmdlist, CMD_SUBSHELL);
			if (!cmd)
				return (cmdlist_destroy(&cmdlist), !SUCCESS);
			if (cmd->cmd_type != CMD_SUBSHELL)
				exit(EXIT_FAILURE);  // syntax error
			cmd->subcommand = token->subshell;
			token->subshell = NULL;
		}
		tokens = tokens->next;
	}
	if (redirs)
	{
		(void)redirs;
		// redirections without commands...
		// iterate through redirlist and create files for > and >>...
	}
	// printf("printing cmdlist...\n");
	// print_commands(&msh->cmdlist);
	return (SUCCESS);
}
