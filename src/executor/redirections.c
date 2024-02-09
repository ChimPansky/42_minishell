/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:39:12 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/09 16:19:24 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "executor.h"
#include "../minishell.h"
#include <sys/stat.h>

// assume always \n at the end
// what if write error?
// pipe buf is 64kb
static int	process_heredoc(
	t_charptr_array *heredoc_lines,
	t_executor *executor)
{
	size_t	line_nb;
	int		pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		return (perror("process_heredoc: pipe"), !SUCCESS);
	line_nb = 0;
	while (line_nb < heredoc_lines->sz)
	{
		if (heredoc_lines->buf[line_nb][0] != '\0')
			ft_dprintf(pipe_fds[WR_END], "%s", heredoc_lines->buf[line_nb]);
		line_nb++;
	}
	close(pipe_fds[WR_END]);
	if (executor->fd_in > 2)
		close(executor->fd_in);
	executor->fd_in = pipe_fds[RD_END];
	return (SUCCESS);
}

static int	check_if_can_be_created(char *fname)
{
	char		*f_dir;

	f_dir = ft_strrchr(fname, '/');
	if (f_dir == NULL)
		f_dir = ft_strdup(".");
	else
		f_dir = ft_substr(fname, 0, f_dir - fname);
	if (!f_dir)
		return (perror("check_permissions: malloc"), !SUCCESS);
	if (access(f_dir, F_OK) != SUCCESS)
		return (free(f_dir),
			ft_printf_err("msh: %s: No such file or directory\n", fname),
			!SUCCESS);
	if (access(f_dir, W_OK) != SUCCESS)
		return (free(f_dir),
			ft_printf_err("msh: %s: Permission denied\n", fname), !SUCCESS);
	return (free(f_dir), SUCCESS);
}

static int	check_permissions(char *fname, bool read_only)
{
	struct stat	fstat;

	if (read_only)
	{
		if (access(fname, F_OK) != SUCCESS)
			return (ft_printf_err("msh: %s: No such file or directory\n",
					fname), !SUCCESS);
		if (access(fname, R_OK) == SUCCESS)
			return (SUCCESS);
		return (ft_printf_err("msh: %s: Permission denied\n", fname), !SUCCESS);
	}
	if (ft_str_end_with(fname, "/"))
		return (ft_printf_err("msh: %s: Is a directory\n", fname), !SUCCESS);
	if (access(fname, F_OK) == SUCCESS)
	{
		if (SUCCESS != stat(fname, &fstat))
			return (perror("check_permissions: stat"), !SUCCESS);
		if (S_ISDIR(fstat.st_mode))
			return (ft_printf_err("msh: %s: Is a directory\n", fname),
				!SUCCESS);
		if (access(fname, W_OK) == SUCCESS)
			return (SUCCESS);
		return (ft_printf_err("msh: %s: Permission denied\n", fname), !SUCCESS);
	}
	return (check_if_can_be_created(fname));
}

static int	process_files(t_redir_detail *redir, t_executor *executor)
{
	if (check_permissions(redir->content.buf[0], redir->type == FD_IN)
		!= SUCCESS)
		return (!SUCCESS);
	if (redir->type == FD_IN)
	{
		if (executor->fd_in > 2)
			close(executor->fd_in);
		executor->fd_in = open(redir->content.buf[0], O_RDONLY);
	}
	else
	{
		if (executor->fd_out > 2)
			close(executor->fd_out);
		if (redir->type == FD_OUT_TRUNC)
			executor->fd_out = open(redir->content.buf[0],
					O_TRUNC | O_CREAT | O_WRONLY, 0644);
		else if (redir->type == FD_OUT_APPEND)
			executor->fd_out = open(redir->content.buf[0],
					O_APPEND | O_CREAT | O_WRONLY, 0644);
	}
	if (executor->fd_in < 0 || executor->fd_out < 0)
		return (perror("process_files: open"), !SUCCESS);
	return (SUCCESS);
}

int	process_redirections(t_executor *executor, t_redirlist *redirs)
{
	t_redir_detail	*redir;
	int				res;

	while (redirs)
	{
		redir = redirs->content;
		if (!redir)
			return (ft_printf_err("CRIT ERR: empty redir details\n"), !SUCCESS);
		if (redir->type == FD_HEREDOC)
			res = process_heredoc(&redir->content, executor);
		else if (redir->content.sz != 1)
			return (ft_printf_err("msh: %s: ambiguous redirction\n",
					redir->string.buf), !SUCCESS);
		else
			res = process_files(redir, executor);
		if (res != SUCCESS)
			return (!SUCCESS);
		redirs = redirs->next;
	}
	return (SUCCESS);
}
