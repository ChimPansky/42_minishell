#include "minishell.h"

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces


// LEXER (input) -> token list
//
// CHECK ERRS:
//  redirect token contains doc
//  bracket check (bracket inside quote case as well)
//  quotes check
//  pipe, end, or, ; at eol
//
// PARSER (token list) -> command_chain
//      expander is part of parser
//      check for correct doc names as well and expand (heredoc delim not expandable)
//
// EXECUTOR


size_t read_shell_spaces(char **input)
{
    size_t  sep_count;

    sep_count = 0;
    while (is_shell_space(**input))
    {
        *(input) += 1;
        sep_count++;
    }
    return (sep_count);
}

int read_word(char **input, t_string *str)
{
    char    quote_type;

    if (string_init(str, "") != SUCCESS)
        return (!SUCCESS);
    quote_type = 0;
    read_shell_spaces(input);
    while (**input)
    {
        if (quote_type == 0 && is_token_seperator(**input))
            break;
        if (quote_type == 0 && (**input == '\'' || **input == '"')) // found opening quote
            quote_type = **input;
        else if (**input == quote_type)  // found closing quote
            quote_type = 0;
        string_add_chr(str, **input);
        (*input) += 1;
    }
    if (quote_type != 0)
        return (string_destroy(str), !SUCCESS);
    return (SUCCESS);
}

t_redir_detail  *read_redir(char **input)
{
    t_redir_detail  *redir_detail;

    redir_detail = malloc(sizeof(t_redir_detail));
    if (!redir_detail)
        return (NULL);
    if (ft_strncmp("<<", *input, 2) == MATCH)
        redir_detail->type = FD_HEREDOC;
    else if (ft_strncmp(">>", *input, 2) == MATCH)
        redir_detail->type = FD_OUT_APPEND;
    else if (**input == '<')
        redir_detail->type = FD_IN;
    else if (**input == '>')
        redir_detail->type = FD_OUT_TRUNC;
    if (redir_detail->type == FD_HEREDOC || redir_detail->type == FD_OUT_APPEND)
        (*input) += 2;
    else
        (*input) += 1;
    return (redir_detail);
}

// turns input into token_list; stores token_list in msh.tokens
int lexer(t_msh *msh, char *input)
{
    t_redir_detail  *redir;
    t_string        str;
    //char            *word;

    // grep NAME < Makefile > out1.txt |
    //only temporary:
    if (ft_strcmp(input, "exit") == MATCH)
        ms_exit(msh, EXIT_SUCCESS);
    while (*input)
    {
        if (*input == '<' || *input == '>')
        {
             if (read_word(&input, &str) != SUCCESS)
            {   // TODO: there was a problem reading the word...
                msh->err_number = ER_UNEXPECTED_TOKEN;
                msh->err_info = "newline";
                return (ERROR);
            }
            redir = read_redir(&input);
            if (redir)
            {
                token_add(&msh->tokens, TK_REDIR, str, redir);
                msh->last_token_type = TK_REDIR;
            }
            else
            {   // TODO: there was a problem reading the redirection...
                msh->err_number = ER_UNEXPECTED_TOKEN;
                msh->err_info = "newline";
                return (ERROR);
            }
        }
        else if (*input == '|')
        {
            if (msh->last_token_type == TK_NULL || msh->last_token_type == TK_PIPE)
            {    // TODO throw syntax error: 2 pipes in a row or pipe is first token...
                msh->err_number = ER_UNEXPECTED_TOKEN;
                msh->err_info = "|";
                return (ERROR);
            }
            token_add(&msh->tokens, TK_PIPE, NULL, NULL); // WHAT TO DO WITH STR HEREEEEEE, ASDNJSDFLJBSDGL
            msh->last_token_type = TK_PIPE;
            input++;
        }
        else if (is_shell_space(*input))
            input++;
        else
        {
            if (read_word(&input, &str) != SUCCESS)
            {   // TODO: there was a problem reading the word...
                msh->err_number = ER_UNEXPECTED_TOKEN;
                msh->err_info = "newline";
                return (ERROR);
            }
            else
            {
                token_add(&msh->tokens, TK_WORD, str, NULL);
                msh->last_token_type = TK_WORD;
            }
        }
    }
    if (msh->last_token_type == TK_PIPE)
        msh->mult_line_input = true;
    else
        msh->mult_line_input = false;
    return (SUCCESS);
}
