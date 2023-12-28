#include "minishell.h"
#include <sys/wait.h>

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces

enum e_fd_type {
    FD_IN,
    FD_HEREDOC,
    FD_OUT_TRUNC,
    FD_OUT_APPEND,
};

enum e_token_type
{
    SEPARATOR,
    TEXT,
    ASSIGNMENT,
    REDIR_IN,
    REDIR_OUT_TRUNC,
    REDIR_OUT_APPEND,
    HEREDOC,
    PIPE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    LOGIC_AND,
    LOGIC_OR,
    DQUOTE_OPEN,
    DQUOTE_CLOSE,
    SQUOTE_OPEN,
    SQUOTE_CLOSE,
};

int execute_in_subshell(t_msh *msh, char **cmd_with_args)
{
    // char **curr_token;

    msh->pid_to_wait = fork();
    if (msh->pid_to_wait != 0) {
        int ret;
        waitpid(msh->pid_to_wait, &ret, 0);
        return ret;
    }
    // printf("IM CHILD\n");
    execute_by_cmd_with_args(msh, cmd_with_args);
    return 0;
}

// tokens gonna be linked list
int parse(t_msh *msh, char *input)
{
    char **tokens = ft_split(input, " \t"); // (ft_split, many seps)
    // int pipefds[2];
    char *cmd_with_args[10] = {};
    int i = 0;

    if (!tokens)
        exit(EXIT_FAILURE);
    // char **cur_start_token = tokens;
    while (*tokens)
    {
        if (ft_strncmp(*tokens, ">", 2) == SUCCESS) {
            // *tokens = "SKIP";
            tokens++;
            // close(msh->out_fd); // in subprocess to not close stdin/out/err
            msh->out_fd = open(*tokens, O_TRUNC | O_CREAT | O_WRONLY, 0644);
            // *tokens = "SKIP";
        } else if (ft_strncmp(*tokens, "<", 2) == SUCCESS) {
            // *tokens = "SKIP";
            tokens++;
            // close(msh->in_fd);
            msh->in_fd = open(*tokens, O_RDONLY);
            // *tokens = "SKIP";
        } else {
            cmd_with_args[i++] = ft_strdup(*tokens);
        }
        tokens++;
    }
    cmd_with_args[i] = NULL;
    t_built_in f = get_built_in_by_name(cmd_with_args[0]);
    if (f != NULL)
        msh->last_exit_code = f(msh, cmd_with_args);
    else
        msh->last_exit_code = execute_in_subshell(msh, cmd_with_args);
    msh->in_fd = STDIN_FILENO;
    msh->out_fd = STDOUT_FILENO;
    msh->err_fd = STDERR_FILENO;
    return 0;
}
