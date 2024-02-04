/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 18:46:26 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TOKENS_H
#define LIST_TOKENS_H

#include "ft_string.h"
#include "ft_list.h"
#include "redirs.h"

typedef t_list t_tokenlist;

// mb add semicolon
typedef enum e_token_type
{
	TK_NULL,
    TK_WORD,
    TK_REDIR,
	TK_PIPE,
	TK_SUBSHELL,
	TK_LOGIC_AND,
    TK_LOGIC_OR
}		t_token_type;

typedef struct s_token
{
	t_token_type		tk_type;
	union {
		t_string		string;
		t_redir_detail	*redir;
		t_tokenlist		*subshell;		//	only for bonus
	};
}		t_token;

int			token_init(t_token **token, t_token_type type);
t_token		*tokenlist_add_token(t_tokenlist **tokenlist, t_token_type type);
void 		token_destroy(void *token_void);
void		tokenlist_destroy(t_tokenlist **tokens);

void	print_tokens(t_tokenlist *tokens);

#endif  // LIST_TOKENS_H
