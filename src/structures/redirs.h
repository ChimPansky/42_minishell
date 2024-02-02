#ifndef REDIRS_H
#define REDIRS_H

#include "libft.h"
#include "ft_string.h"
#include "ft_charptr_array.h"

// redirlist doesn't own redir_details!
// details should be free through tokens!
typedef t_list t_redirections;

typedef enum e_redir_type
{
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
}		t_redir_detail;

#endif  // REDIRS_H
