#include "minishell.h"

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces

enum fd_type {
    IN_FILE,
    HEREDOC,
    OUT_FILE_TRUNK,
    OUT_FILE_APPEND,
};

// tokens gonna be linked list
int parse(char *input)
{
    char **tokens = ft_split(input, ' '); // (ft_split, many seps)
    int pipefds[2];
    if (!tokens)
        exit(EXIT_FAILURE);

    char **cur_start_token = tokens;
    while (*tokens)
    {
        if (ft_strncmp(*tokens, ">", 2) == SUCCESS) {
            *tokens = "SKIP";
            tokens++;
            close(msh.in_fd);
            msh.out_fd = open(*tokens, O_TRUNC | O_CREAT | O_WRONLY, 0644);
            dup2(fd, STDOUT_FILENO);
            *tokens = "SKIP";
        } else if (ft_strncmp(*tokens, "<", 2) == SUCCESS) {
            *tokens = "SKIP";
            tokens++;
            close(msh.in_fd);
            msh.in_fd = open(*tokens, O_RDONLY);
            *tokens = "SKIP";
        } else if (ft_strncmp(*tokens, "|", 2)) {
            pipe(pipefds);
            close(msh.in_fd);
            msh.in_fd = pipefds[0];
            exec(msh, cmd_with_args_from_tokens(curr_start_token, tokens));
            close(msh.out_fd);
            msh.out_fd = pipefds[1];
            dup2(pipefds[1], STDIN_FILENO);
            cur_start_token = tokens + 1;
        }
        tokens++;
    }
    find builtin
    exec(curr_start_token, tokens);
    in_fd = STDIN_FILENO;
    out_fd = STDOUT_FILENO;
}
