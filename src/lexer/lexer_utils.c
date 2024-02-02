/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:36:38 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/02 14:41:19 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

size_t read_shell_spaces(char **input)
{
	size_t  sep_count;

	sep_count = 0;
	while (is_shell_space(**input))
	{
		*(input) += 1;
		sep_count++;
	}
	return (sep_count);
}
