#include "libft.h"
#include "executor.h"
#include "../minishell.h"

// assume always \n at the end
// what if write error? possibly write in different process, though it is fucked up
// pipe buf is 64kb
static int process_heredoc(t_charptr_array *heredoc_lines, t_executor *executor)
{
	size_t	line_len;
	size_t	line_nb;
	int		pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		return (perror("heredoc pipe"), !SUCCESS);
	line_nb = 0;
	while (line_nb < heredoc_lines->sz)
	{
		line_len = ft_strlen(heredoc_lines->buf[line_nb]);
		if (line_len)
			write(pipe_fds[WR_END], heredoc_lines->buf[line_nb], line_len);
		line_nb++;
	}
	close(pipe_fds[WR_END]);
	if (executor->fd_in > 2)
		close(executor->fd_in);
	executor->fd_in = pipe_fds[RD_END];
	return SUCCESS;
}

// TODO directory and other checks (cd as example)
static int process_files(t_redir_detail *redir, t_executor *executor)
{
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
			executor->fd_out = open(redir->content.buf[0], O_TRUNC | O_CREAT | O_WRONLY, 0644);
		else if (redir->type == FD_OUT_APPEND)
			executor->fd_out = open(redir->content.buf[0], O_APPEND | O_CREAT | O_WRONLY, 0644);
	}
	if (executor->fd_in < 0 || executor->fd_out < 0)
		return (perror(redir->content.buf[0]), !SUCCESS);
	return SUCCESS;
}

static int process_redirection(t_redir_detail *redir, t_executor *executor)
{
	if (redir->type == FD_HEREDOC)
		return process_heredoc(&redir->content, executor) != SUCCESS;
	else if (redir->content.sz != 1)
		return (ft_printf_err("msh: %s: ambiguous redirction\n", redir->string.buf), !SUCCESS);
	return process_files(redir, executor);
}

int process_redirections(t_executor *executor, t_redirlist *redirs)
{
	while (redirs)
	{
		if (process_redirection(redirs->content, executor) == !SUCCESS)
			return (!SUCCESS);
		redirs = redirs->next;
	}
	return SUCCESS;
}
