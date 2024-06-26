/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_variables.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 00:20:50 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/10 00:25:55 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_VARIABLES_H
# define LIST_VARIABLES_H

# include "ft_charptr_array.h"
# include "ft_list.h"

typedef struct s_var
{
	char	*name;
	char	*value;
}		t_var;

typedef t_list	t_varlist;

char		*varlist_get_value_by_substr(
				t_varlist *vars, const char *str, size_t sz);
char		*varlist_get_value(t_varlist *vars, const char *name);
t_var		*varlist_find(t_varlist *vars, const char *name);
t_var		*varlist_set(
				t_varlist **vars_p, const char *name, const char *value);
void		varlist_delete_one(t_varlist **vars_p, const char *name);
void		varlist_destoy(t_varlist **vars_p);

t_varlist	*varlist_init_from_envp(char **envp);
// on fail returns null
int			varlist_convert_to_array(t_varlist *vars, t_charptr_array *arr);

#endif  // LIST_VARIABLES_H
