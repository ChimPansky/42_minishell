/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:36 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 18:54:42 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

t_built_in	get_built_in_by_name(char *func_name)
{
	if (strcmp(func_name, "echo") == SUCCESS)
		return (built_in_echo);
	if (strcmp(func_name, "cd") == SUCCESS)
		return (built_in_cd);
	if (strcmp(func_name, "pwd") == SUCCESS)
		return (built_in_pwd);
	if (strcmp(func_name, "export") == SUCCESS)
		return (built_in_export);
	if (strcmp(func_name, "unset") == SUCCESS)
		return (built_in_unset);
	if (strcmp(func_name, "env") == SUCCESS)
		return (built_in_env);
	if (strcmp(func_name, "exit") == SUCCESS)
		return (built_in_exit);
	return (NULL);
}
