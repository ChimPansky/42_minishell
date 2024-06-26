/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:14:09 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_tokens.h"
#include "redirs.h"
#include "libft.h"
#include <unistd.h>

int	token_init(t_token **token, t_token_type type)
{
	*token = malloc(sizeof(t_token));
	if (!token)
		return (!SUCCESS);
	ft_bzero(*token, sizeof(t_token));
	(*token)->tk_type = type;
	if (type == TK_WORD)
	{
		if (string_init(&(*token)->string, "") != SUCCESS)
			return (free(*token), !SUCCESS);
	}
	return (SUCCESS);
}

t_token	*tokenlist_add_token(t_tokenlist **tokenlist, t_token_type type)
{
	t_token		*token;
	t_tokenlist	*new_node;

	if (token_init(&token, type) != SUCCESS)
		return (NULL);
	new_node = ft_lstnew(token);
	if (!new_node)
		return (token_destroy(token), NULL);
	ft_lstadd_back(tokenlist, new_node);
	return (token);
}

void	token_destroy(void *token_void)
{
	t_token	*token;

	token = token_void;
	if (token->tk_type == TK_REDIR)
		redir_destroy(token->redir);
	if (token->tk_type == TK_WORD)
		string_destroy(&token->string);
	if (token->tk_type == TK_SUBSHELL)
		tokenlist_destroy(&token->subshell_tokens);
	free(token);
}

void	tokenlist_destroy(t_tokenlist **tokens)
{
	ft_lstclear(tokens, token_destroy);
}

bool	is_cmd_separator_token(t_token_type tk_type)
{
	return (false
		|| tk_type == TK_LOGIC_OR
		|| tk_type == TK_LOGIC_AND
		|| tk_type == TK_NULL);
}

/*
void	print_tokens(t_tokenlist *tokens)
{
	t_token		*token;

	char		*type_text;
	char		*fd_type;
	char		*fd_str;


	int			i = 1;

	printf("token_list:\n");
	while (tokens)
	{
		token = tokens->content;
		if (token->tk_type == TK_WORD)
		{
			type_text = "WORD";
			fd_type = NULL;
			fd_str = NULL;
		}
		else if (token->tk_type == TK_REDIR)
		{
			type_text = "REDIR";
			if (token->redir->string.buf)
				fd_str = token->redir->string.buf;
			else
				fd_str = NULL;
			if (token->redir->type == FD_IN)
				fd_type = "<";
			else if (token->redir->type == FD_HEREDOC)
				fd_type = "<<";
			else if (token->redir->type == FD_OUT_TRUNC)
				fd_type = ">";
			else if (token->redir->type == FD_OUT_APPEND)
				fd_type = ">>";
		}
		else if (token->tk_type == TK_PIPE)
		{
			type_text = "PIPE";
			fd_type = NULL;
			fd_str = NULL;
		}
		else if (token->tk_type == TK_LOGIC_AND)
		{
			type_text = "LOGIC_AND";
			fd_type = NULL;
			fd_str = NULL;
		}
		else if (token->tk_type == TK_LOGIC_OR)
		{
			type_text = "LOGIC_OR";
			fd_type = NULL;
			fd_str = NULL;
		}
		else if (token->tk_type == TK_SUBSHELL)
		{
			type_text = "SUBSHELL";
			fd_type = NULL;
			fd_str = NULL;
		}
		printf(
		" {T%d: Type: %s; t_string: %s; FD_Type: %s; FD_t_string: %s)} -->\n",
		i, type_text, token->string.buf, fd_type, fd_str);
		if (token->tk_type == TK_SUBSHELL)
		{
			type_text = "SUBSHELL";
			fd_type = NULL;
			fd_str = NULL;
			printf ("\n---SUB-TOKENS START\n");
			print_tokens(token->subshell_tokens);
			printf ("---SUB-TOKENS END\n");
		}
		// if (token->redir->type == FD_HEREDOC && token->redir->content.buf)
		// 	charptr_array_print(&token->redir->content);
		tokens = tokens->next;
		i++;
	}
	printf("\n");
}
*/
