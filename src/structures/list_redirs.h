/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_redirs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:52:34 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 15:19:16 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_REDIRS_H
# define LIST_REDIRS_H

# include "libft.h"
# include "ft_string.h"

typedef t_list t_redirlist;

typedef enum e_redir_type
{
    FD_IN,
    FD_HEREDOC,
    FD_OUT_TRUNC,
    FD_OUT_APPEND
}		t_redir_type;

// str: unexpanded filename or delimiter (only heredoc)
// content: either expanded filename or heredoc lines
typedef struct s_redir_detail
{
	t_redir_type	type;
	t_string		*str;
	char			**content;
}		t_redir_detail;

t_redir_detail	*redir_add(t_redirlist **redirs, t_redir_type rd_type,
                t_string *rd_content, t_string *rd_delimiter);
void			redir_destroy(void *redir_void);
void			redirlist_destroy(t_redirlist **redirs);

#endif  // LIST_REDIRS_H
