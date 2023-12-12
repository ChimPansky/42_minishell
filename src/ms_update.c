/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:49:31 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/12 23:26:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_update(t_minish *minish)
{
	getcwd(minish->pwd, PATH_MAX);
	if (minish->prompt)
		free_null((void **)&minish->prompt);
	minish->prompt = ft_strjoin(minish->pwd, ": ");
	if (!minish->prompt)
		ms_error_exit(minish, ERR_MALLOC);
}
