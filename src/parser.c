#include "minishell.h"
#include <sys/wait.h>

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces



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
bool    is_shell_space(char c)
{
    if (ft_strchr(MS_WHITESPACES, c))
        return (true);
    return (false);
}

bool    is_word_sep(char c)
{
    return (is_shell_space(c)); // add other seperators like = | &&...
}

int token_add_redir(t_msh *msh, t_tokens *tokens, char *input, int *i)
{
    
    //strcmp(input, ">>") == SUCCESS
}

// TODO: rewrite new_parse...
int new_parse(t_msh *msh, t_tokens *tokens, char *input)
{
    int     i;
    bool    in_squotes;
    bool    in_dquotes;
    bool    in_brackets;
    bool    in_word;
    size_t  start;
    char    *word;

    i = 0;
    in_squotes = false;
    in_dquotes = false;
    in_brackets = false;
    in_word = false;
    start = 0;
    while (*input)
    {
        if ((in_squotes && *input == '\'') || (in_dquotes && *input == '"') || (in_word && is_word_sep(*input)))
        {
            in_squotes = false;
            in_dquotes = false;
            in_word = false;
            word = ft_substr(input, start, i - start);
            if (!word)
                ms_exit(msh, ERROR);    // TODO ERROR
            token_add(&tokens, TK_WORD, 0, word);
        }
        else if (!in_squotes && !in_dquotes && !in_word && (*input == '\'' || *input == '"'))
        {
            start = i + 1;
            if (*input == '\'')
                in_squotes = true;
            else
                in_dquotes = true;
        }
        if (*input == '=')
            token_add(&tokens, TK_ASSIGNMENT, 0, NULL);
        if (*input == '<' || *input == '>')
            token_add_redir(msh, tokens, &input, &i)
        if (*input == '|')
            token_add(&msh->tokens, TK_PIPE, NULL);
        // BONUS:
        if (*input == '*')
            token_add(&msh->tokens, TK_WILDCARD, NULL);
        if (*input == '(')
            token_add(&msh->tokens, TK_BRACKET_OPEN, NULL);
        if (*input == ')')
            token_add(&msh->tokens, TK_BRACKET_CLOSE, NULL);
        if (strcmp(input, "&&") == SUCCESS)
        {
            token_add(&msh->tokens, TK_LOGIC_AND, NULL);
            input += 1;
        }
        if (strcmp(input, "||") == SUCCESS)
        {
            token_add(&msh->tokens, TK_LOGIC_OR, NULL);
            input += 1;
        }
        input++;
        i++;
    }
    print_tokens(&msh->tokens);
    return (0);
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
    print_splitted(cmd_with_args);
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
