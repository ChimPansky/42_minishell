/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:33:12 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "../minishell.h"

int	expand(t_msh *msh, t_tokenlist *tokens);
int	string_expand(t_msh *msh, t_string *str, bool *ws_expansion);
int	read_var_name(char **to_expand, t_string *var_name);

#endif  // EXPANDER_H
