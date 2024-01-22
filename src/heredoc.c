# include "minishell"
// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case
void	process_here_doc(char **document, char *limiter)
{
    char *line;
    char *temp;
    size_t line_sz;
    *document = malloc(sizeof(char));
    if (!*document) {
        return !SUCCESS;
    }
    **document = '\0';
	while (1)
	{
        // does it have \n in the end?
        // if no change strjoin to strnjoin with \n
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
		if (ft_strncmp(limiter, line, line_sz - 1) == MATCH)
			break ;
		temp = ft_strjoin(*document, line);
        if (!temp)
            return(free(line), free(*document), !SUCCESS);
        (free(*document), free(line));
        *document = temp;
	}
}
