/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:34:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 11:26:20 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_redirs.h"
#include "libft.h"

void	redir_destroy(void *redir_void)
{
	t_redir_detail *redir = redir_void;

	string_destroy(&redir->content);
	string_destroy(&redir->delimiter);
	free(redir);
}

void redirlist_destroy(t_redirlist **redirs)
{
	ft_lstclear(redirs, redir_destroy);
}

t_redir_detail	*redir_create(void)
{
	t_redir_detail	*new_redir;

	new_redir = malloc(sizeof(t_redir_detail));
	return (new_redir);
}

int	redir_init(t_redir_detail *redir, t_redir_type rd_type, t_string)
{

}

t_redir_detail	*redir_add_empty(t_redirlist **redirs, t_redir_type rd_type, t_string *rd_content, t_string *rd_delimiter)
{
	t_redir_detail	*redir;
	t_redirlist 	*new_redir;

	redir = malloc(sizeof(t_redir_detail));
	if (!redir)
		return (NULL);
	ft_bzero(redir, sizeof(redir));
	redir->type = rd_type;
	redir->content = *rd_content;
	redir->delimiter = *rd_delimiter;
	new_redir = ft_lstnew(redir);
	if (!new_redir)
		return (redir_destroy(redir), NULL);
	ft_lstadd_back(redirs, new_redir);
	return (redir);
}
