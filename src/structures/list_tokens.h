/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 19:52:02 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TOKENS_H
# define LIST_TOKENS_H

# include "libft.h"
# include "ft_string.h"
# include "list_redirs.h"

typedef t_list t_tokenlist;

typedef enum e_token_type
{
	TK_NULL,
    TK_WORD,
    TK_REDIR,
	TK_PIPE,
	TK_BRACKET,
	TK_LOGIC_AND,
    TK_LOGIC_OR
}		t_token_type;

typedef struct s_token
{
	t_token_type		tk_type;
	union {
		t_string		string;
		t_redir_detail	redir;
		t_tokenlist		*subshell;		//	only for bonus
		int				sub_exit_code;	//	only for bonus
	};
}	t_token;

t_token		*token_add(t_tokenlist **tokens, t_token_type tk_type,
						t_string *str, t_redir_detail *redir);
void tokenlist_destroy(t_tokenlist **tokens);
void 		token_destroy(void *token_void);
void		print_tokens(t_tokenlist **tokens);


#endif  // LIST_TOKENS_H
