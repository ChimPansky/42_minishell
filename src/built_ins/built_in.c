/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:36 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/18 08:55:35 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

t_built_in get_built_in_by_name(char *func_name)
{
    if (strcmp(func_name, "echo") == 0)
		return (built_in_echo);
    if (strcmp(func_name, "cd") == 0)
		return (built_in_cd);
    if (strcmp(func_name, "pwd") == 0)
		return (built_in_pwd);
    if (strcmp(func_name, "export") == 0)
		return (built_in_export);
    if (strcmp(func_name, "unset") == 0)
		return (built_in_unset);
    if (strcmp(func_name, "env") == 0)
		return (built_in_env);
    if (strcmp(func_name, "exit") == 0)
		return (built_in_exit);
	return NULL;
}
