# include "libft.h"

// cmd d and cmd c check
// if success return SUCCESS
int	process_here_doc(char **document, char *limiter)
{
    char *line;
    size_t line_sz;
	while (1)
	{
		line = read_line(STDIN_FILENO); // addhistory
        line_sz = ft_strlen(line);
		if (line == NULL)
        {
            if (errno)
                //errno...
                return !SUCCESS;
            else
            // ^D message and exit
                return !SUCCESS;
        }
		if (ft_strncmp(limiter, line, line_sz - 1) == 0)
			break ;
		free(line.buf));
	}
	free(line.buf);
	close(args->pipe_fds[W_END]);
	args->in_fd = args->pipe_fds[R_END];
}
