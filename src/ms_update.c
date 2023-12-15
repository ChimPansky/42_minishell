/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:49:31 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 14:18:14 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_update_pwd(t_msh *minish)
{
	getcwd(minish->pwd, PATH_MAX);
}

void	ms_update_prompt(t_msh *minish)
{
	getcwd(minish->pwd, PATH_MAX);
	if (minish->prompt)
		free_null((void **)&minish->prompt);
	if (minish->pwd[0])
		minish->prompt = ft_strjoin(minish->pwd, ": ");
	else
		minish->prompt = ft_strjoin("", ": ");
	if (!minish->prompt)
		ms_exit(minish, EXIT_FAILURE);
}

void	ms_update(t_msh *minish)
{
	ms_update_pwd(minish);
	ms_update_prompt(minish);
}
