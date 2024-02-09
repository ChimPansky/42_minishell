/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:18:58 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 18:56:51 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#define BUILT_IN_EXIT_NONUMERIC		2
#define BUILT_IN_EXIT_TOO_MANY_ARGS	1

int		built_in_echo(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_cd(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_pwd(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_export(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_unset(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_env(t_msh *msh, char **cmd_with_args, int fd_out);
int		built_in_exit(t_msh *msh, char **cmd_with_args, int fd_out);
