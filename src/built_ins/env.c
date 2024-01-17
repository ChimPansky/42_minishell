/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:33:15 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/17 15:43:43 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	built_in_env(t_msh *msh, char **cmd_with_args)
{
	(void)msh;
	(void)cmd_with_args;

	char** key_value_pairs;
	key_value_pairs = vars_convert_to_array(msh->env);
	if (!key_value_pairs)	// error_handling...
		return (1);
	while (key_value_pairs && *key_value_pairs)
		printf("%s\n", *key_value_pairs++);
	// free char **key_value_pairs...
	return (0);
}
