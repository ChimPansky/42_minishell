/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:36 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 17:19:26 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

t_built_in get_built_in_by_name(char *func_name)
{
    if (strncmp(func_name, "echo", 5) == 0)
		return (built_in_echo);
    if (strncmp(func_name, "cd", 2) == 0)
		return (built_in_cd);
    if (strncmp(func_name, "pwd", 3) == 0)
		return (built_in_pwd);
    if (strncmp(func_name, "export", 6) == 0)
		return (built_in_export);
    if (strncmp(func_name, "unset", 5) == 0)
		return (built_in_unset);
    if (strncmp(func_name, "env", 3) == 0)
		return (built_in_env);
    if (strncmp(func_name, "exit", 4) == 0)
		return (built_in_exit);
	return NULL;
}
