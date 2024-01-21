#ifndef EXECUTOR_H
#define EXECUTOR_H

# include "../minishell.h"

# define EX_FAILURE -2
# define WR_END 0
# define RD_END 1

typedef struct s_executor
{
	int pipe_fds[2];
	int fd_in;
	int fd_out;
	int pid;
}		t_executor;

int process_redirection(t_redir_detail *redir, int *fd_in, int *fd_out);

#endif  // EXECUTOR_H
