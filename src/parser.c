/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:30:33 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/12 13:51:01 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes list of tokens and turns it into list of one or several commands (command chain); Also: perform variable expansions...
int 	parser(t_msh *msh, t_tokens **tokens, t_command_chain *cmds)
{
	(void) msh;
	(void) tokens;
	(void) cmds;
	printf("parsing tokens (with expansion) into command chain...\n");
	return (0);
}
