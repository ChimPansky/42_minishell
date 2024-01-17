/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 12:28:07 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 11:17:36 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy(t_msh *minish)
{
	if (minish->rl_input)
		free_null((void **)&minish->rl_input);
	if (minish->prompt)
		free_null((void **)&minish->prompt);
	// free t_variables env...
	// free t_variables locals...
}
