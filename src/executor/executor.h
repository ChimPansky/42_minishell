/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:40:18 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/08 19:48:29 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"

# define RD_END 0
# define WR_END 1
# define BROKEN_PIPE -1

typedef struct s_executor
{
	int		fd_in;
	int		fd_out;
	int		num_of_cmds_in_pipe;
	pid_t	*pids;
	bool	is_parent;

}		t_executor;

int		process_redirections(t_executor *executor, t_redirlist *redirs);
int		execute_on_chain(t_msh *msh, t_executor *executor, t_cmdlist *cmds);
void	wait_with_check(t_executor *executor, int *last_exit_code);
int		execute_in_child_process(t_msh *msh, char **cmd_with_args);

typedef struct s_path_finder
{
	const char	*path;
	const char	*exec;
	char		*exec_with_path;
}		t_path_finder;

int		init_path_finder(t_msh *msh, t_path_finder *pf, char *exec_name);
void	destroy_path_finder(t_path_finder *pf);

#endif  // EXECUTOR_H
