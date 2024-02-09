/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:24:34 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirs.h"
#include "libft.h"
#include <unistd.h>

int	redir_init(t_redir_detail **redir, t_redir_type rd_type)
{
	*redir = malloc(sizeof(t_redir_detail));
	if (!redir)
		return (!SUCCESS);
	ft_bzero(*redir, sizeof(t_redir_detail));
	(*redir)->type = rd_type;
	if (string_init(&(*redir)->string, "") != SUCCESS)
		return (free(*redir), !SUCCESS);
	if (charptr_array_init(&(*redir)->content) != SUCCESS)
		return (string_destroy(&(*redir)->string), free(*redir), !SUCCESS);
	return (SUCCESS);
}

void	redir_destroy(void *redir_void)
{
	t_redir_detail	*redir;

	if (!redir_void)
		return ;
	redir = redir_void;
	string_destroy(&redir->string);
	charptr_array_destroy(&redir->content);
	free(redir);
}

void	redirlist_destroy(t_redirlist **redirs)
{
	ft_lstclear(redirs, NULL);
}

// void	print_redirs(t_list **redirections)
// {
// 	t_redirlist	*cur_redir;
// 	t_redir_detail	*redir;
// 	char			*fd_type;
// 	int			i;

// 	i = 1;
// 	cur_redir = *redirections;
// 	while (cur_redir)
// 	{
// 		redir = cur_redir->content;
// 		if (redir->type == FD_IN)
// 			fd_type = "<";
// 		else if (redir->type == FD_HEREDOC)
// 			fd_type = "<<";
// 		else if (redir->type == FD_OUT_TRUNC)
// 			fd_type = ">";
// 		else if (redir->type == FD_OUT_APPEND)
// 			fd_type = ">>";
// 		printf("%d:\n", i);
// 		printf("type: %s\n", fd_type);
// 		printf("str: %s\n", redir->string.buf);
// 		cur_redir = cur_redir->next;
// 		i++;
// 	}
// }
