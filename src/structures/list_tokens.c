/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 11:16:34 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_tokens.h"
#include "libft.h"

void	token_destroy(void *token_void)
{
	t_token *token = token_void;

	if (token->tk_type == TK_REDIR)
		redir_destroy(&token->redir);
	if (token->tk_type == TK_WORD)
		string_destroy(&token->string);
	free(token);
}

void tokenlist_destroy(t_tokenlist **tokens)
{
	ft_lstclear(tokens, token_destroy);
}

t_token		*token_add(t_tokenlist **tokens, t_token_type tk_type,
						t_string *str, t_redir_detail *redir)
{
	t_token 	*token;
	t_tokenlist	*new_token;

	token = malloc(sizeof(t_token));	//ask vova why its not sizeof(t_token *)...
	if (!token)
		return (NULL);
	ft_bzero(token, sizeof(token));
	token->tk_type = tk_type;
	if (tk_type == TK_REDIR)
		token->redir = *redir;
	else if (tk_type == TK_WORD)
		token->string = *str;
	new_token = ft_lstnew(token);
	if (!new_token)
		return (token_destroy(token), NULL);
	ft_lstadd_back(tokens, new_token);
	return (token);
}

void	print_tokens(t_tokenlist **tokens)
{
	t_tokenlist	*cur_list;
	t_token		*cur_token;
	char		*type_text;
	char		*tk_str;
	char		*fd_type;
	char		*fd_content;
	char		*fd_delimiter;
	int			i;

	if (!tokens)
		return ;
	printf("token_list:\n");
	cur_list = *tokens;
	i = 1;
	while (cur_list && cur_list->content)
	{
		cur_token = cur_list->content;
		if (cur_token->string.buf)
			tk_str = cur_token->string.buf;
		else
			tk_str = NULL;
		if (cur_token->tk_type == TK_WORD)
		{
			type_text = "WORD";
			fd_type = NULL;
			fd_content = NULL;
			fd_delimiter = NULL;
		}
		else if (cur_token->tk_type == TK_REDIR)
		{
			type_text = "REDIR";
			if (cur_token->redir.delimiter.buf)
				fd_delimiter = cur_token->redir.delimiter.buf;
			else
				fd_delimiter = NULL;
			if (cur_token->redir.content.buf)
				fd_content = cur_token->redir.content.buf;
			else
				fd_content = NULL;
			if (cur_token->redir.type == FD_IN)
				fd_type = "<";
			else if (cur_token->redir.type == FD_HEREDOC)
				fd_type = "<<";
			else if (cur_token->redir.type == FD_OUT_TRUNC)
				fd_type = ">";
			else if (cur_token->redir.type == FD_OUT_APPEND)
				fd_type = ">>";
		}
		else if (cur_token->tk_type == TK_PIPE)
		{
			type_text = "PIPE";
			fd_type = NULL;
			fd_content = NULL;
			fd_delimiter = NULL;
		}

		printf(" {T%d: Type: %s; t_string: %s; FD_Type: %s; FD_content: %s; FD_Delimiter: %s)} -->\n",  i, type_text, tk_str, fd_type, fd_content, fd_delimiter);
		cur_list = cur_list->next;
		i++;
	}
	printf("\n");
}
