/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:36 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 21:20:53 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

t_built_in	get_built_in_by_name(char *func_name)
{
	if (ft_strcmp(func_name, "echo") == SUCCESS)
		return (built_in_echo);
	if (ft_strcmp(func_name, "cd") == SUCCESS)
		return (built_in_cd);
	if (ft_strcmp(func_name, "pwd") == SUCCESS)
		return (built_in_pwd);
	if (ft_strcmp(func_name, "export") == SUCCESS)
		return (built_in_export);
	if (ft_strcmp(func_name, "unset") == SUCCESS)
		return (built_in_unset);
	if (ft_strcmp(func_name, "env") == SUCCESS)
		return (built_in_env);
	if (ft_strcmp(func_name, "exit") == SUCCESS)
		return (built_in_exit);
	return (NULL);
}
