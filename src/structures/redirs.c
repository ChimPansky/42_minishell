/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 20:15:45 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirs.h"
#include "libft.h"

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
	if (!redir_void)
		return ;
	t_redir_detail	*redir = redir_void;

	string_destroy(&redir->string);
	charptr_array_destroy(&redir->content);
	free(redir);
}

int redirlist_add_redir(t_redirlist **redirlist, t_redir_detail *redir)
{
	t_redirlist *new_node = ft_lstnew(redir);
	if (!new_node)
		return (!SUCCESS);
	ft_lstadd_back(redirlist, new_node);
	return (SUCCESS);
}

// t_redir_detail *redirlist_add_redir(t_redirlist **redirlist)
// {
// 	t_redir_detail *redir;

// 	if (SUCCESS != redir_init(&redir, FD_NULL))
// 		return NULL;
// 	t_redirlist *new_node = ft_lstnew(redir);
// 	if (!new_node)
// 		return (redir_destroy(&redir), NULL);
// 	ft_lstadd_back(redirlist, new_node);
// 	return redir;
// }

void	redirlist_destroy(t_redirlist **redirs)
{
	ft_lstclear(redirs, NULL);
}
