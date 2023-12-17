/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:58 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 17:37:16 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		built_in_echo(t_msh *msh, char **cmd_with_args);
int		built_in_cd(t_msh *msh, char **cmd_with_args);
int		built_in_pwd(t_msh *msh, char **cmd_with_args);
int		built_in_export(t_msh *msh, char **cmd_with_args);
int		built_in_unset(t_msh *msh, char **cmd_with_args);
int		built_in_env(t_msh *msh, char **cmd_with_args);
int		built_in_exit(t_msh *msh, char **cmd_with_args);
