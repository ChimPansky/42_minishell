/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:25:17 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 17:34:21 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRS_H
# define REDIRS_H

#include "ft_string.h"
#include "ft_charptr_array.h"
#include "ft_list.h"
#include <stdbool.h>

// redirlist doesn't own redir_details!
// details should be free through tokens!
typedef t_list t_redirlist;

typedef enum e_redir_type
{
	FD_NULL,
    FD_IN,
    FD_HEREDOC,
    FD_OUT_TRUNC,
    FD_OUT_APPEND
}		t_redir_type;

typedef struct s_redir_detail
{
	t_redir_type	type;
	t_string		string;
	t_charptr_array content;
	bool			expand_heredoc;
}		t_redir_detail;

int		redir_init(t_redir_detail **redir, t_redir_type rd_type);
void	redir_destroy(void *redir_void);
void	redirlist_destroy(t_redirlist **redirs);

#endif  // REDIRS_H
