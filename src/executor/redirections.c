#include "libft.h"
#include "executor.h"
#include "../minishell.h"

// assume always \n at the end
// what if write error?
static int process_heredoc(char *str)
{
	char *eol_pos;

	int pipe_fds[2];
	if (pipe(pipe_fds) < 0)
		return (perror("heredoc pipe"), EX_FAILURE);
	eol_pos = ft_strchr(str, '\n');
	while (eol_pos)
	{
		write(pipe_fds[WR_END], str, eol_pos - str + 1);
		str = eol_pos + 1;
		eol_pos = ft_strchr(str, '\n');
	}
	close(pipe_fds[WR_END]);
	return pipe_fds[RD_END];
}

static int process_redirection(t_redir_detail *redir, t_executor *executor)
{
	if (redir->type == FD_HEREDOC)
	{
		if (executor->fd_in > 2)
			close(executor->fd_in);
		executor->fd_in = process_heredoc(redir->str);
		if (executor->fd_in < 0)
			return (EX_FAILURE);
	}
	else if (redir->type == FD_IN)
	{
		if (executor->fd_in > 2)
			close(executor->fd_in);
		executor->fd_in = open(redir->str, O_RDONLY);
		if (executor->fd_in < 0)
			return (perror(redir->str), EX_FAILURE);
	}
	else
	{
		if (executor->fd_out > 2)
			close(executor->fd_out);
		if (redir->type == FD_OUT_TRUNC)
			executor->fd_out = open(redir->str, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		else
			executor->fd_out = open(redir->str, O_APPEND | O_CREAT | O_WRONLY, 0644);
		if (executor->fd_out < 0)
			return (perror(redir->str), EX_FAILURE);
	}
	return SUCCESS;
}

int process_redirections(t_executor *executor, t_redirections *redirs)
{
	while (redirs)
	{
		if (process_redirection(redirs->content, executor) == EX_FAILURE)
			return (EX_FAILURE);
		redirs = redirs->next;
	}
	return SUCCESS;
}
