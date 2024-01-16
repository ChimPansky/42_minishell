#include "minishell.h"

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces



// int token_add_redir(t_msh *msh, t_tokens *tokens, char *input, int *i)
// {

//     //strcmp(input, ">>") == SUCCESS
// }

/*
llist of long cmds:

llist to executor:
long command:

simple command:
char **words,
llist of t_redir_descr
*/


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


// TODO: MS_WHITESPACES AND MS_SPECIAL_CHARS are macros in ms_macros...
// should we store info somewhere else?
bool    is_shell_space(char c)
{
    if (c && ft_strchr(MS_WHITESPACES, c))
        return (true);
    return (false);
}

bool    is_shell_special(char c)
{
    if (c && ft_strchr(MS_SPECIAL_CHARS, c))
        return (true);
    return (false);
}

bool    is_word_sep(char c)
{
    if (!c || is_shell_space(c) || is_shell_special(c))
        return (true);
    return (false);
}

size_t read_shell_seps(char **input)
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

char    *add_to_word(char **word, char new_char)
{
    char    *new_word;
    char    appendix[2];

    appendix[0] = new_char;
    appendix[1] = '\0';
    if (!*word)
        new_word = ft_strdup(appendix);
    else
    {
        new_word = ft_strjoin(*word, appendix);
        free_null((void**)word);
    }
    return (new_word);
}

char    *read_word(char **input)
{
    char    *word;
    char    in_quotes;

    word = NULL;
    in_quotes = 0;
    read_shell_seps(input);
    while (**input)
    {
        if (!in_quotes && is_word_sep(**input))
            break;
        else if (!in_quotes && (**input == '\'' || **input == '"')) // found opening quote
            in_quotes = **input;
        else if (**input == in_quotes)  // found closing quote
            in_quotes = 0;
        else
        {
            word = add_to_word(&word, **input);
            if (!word)
                return (NULL);
        }
        (*input) += 1;
    }
    if (in_quotes)
        return (free_null((void**)&word), NULL);
    return (word);
}

t_redir_detail  *read_redir(char **input)
{
    t_redir_detail  *redir_detail;

    redir_detail = malloc(sizeof(t_redir_detail));
    if (!redir_detail)
        return (NULL);
    if (ft_strncmp("<<", *input, 2) == SUCCESS)
        redir_detail->type = FD_HEREDOC;
    else if (ft_strncmp(">>", *input, 2) == SUCCESS)
        redir_detail->type = FD_OUT_APPEND;
    else if (**input == '<')
        redir_detail->type = FD_IN;
    else if (**input == '>')
        redir_detail->type = FD_OUT_TRUNC;
     ft_putstr_fd(*input, 1);
    if (redir_detail->type == FD_HEREDOC || redir_detail->type == FD_OUT_APPEND)
        (*input) += 2;
    else
        (*input) += 1;
    read_shell_seps(input);
    redir_detail->str = read_word(input);
    if (redir_detail->str)      // TODO syntax error!
        return (redir_detail);
    return (free(redir_detail), NULL);
}

// turns input into token_list; stores token_list in msh.tokens
int lexer(t_msh *msh, char *input)
{
    t_redir_detail  *redir;
    char            *word;

    // grep NAME < Makefile > out1.txt |
    //only temporary:
    if (ft_strcmp(input, "exit") == SUCCESS)
        ms_exit(msh, EXIT_SUCCESS);
    if (ft_strcmp(input, "pipe") == SUCCESS)
         msh->last_token = token_add(&msh->tokens, TK_PIPE, 0, NULL);
    while (*input)
    {
        if (*input == '<' || *input == '>')
        {
            redir = read_redir(&input);
            if (redir)
                msh->last_token = token_add(&msh->tokens, TK_REDIR, NULL, redir);
            else
            {   // TODO: there was a problem reading the redirection...
                msh->err_syntax = true;
                msh->unexpected_token = "newline";
                break;
            }
        }
        else if (*input == '|')
        {
            if (!msh->last_token || msh->last_token->tk_type == TK_PIPE)
            {    // TODO throw syntax error: 2 pipes in a row or pipe is first token...
                msh->err_syntax = true;
                msh->unexpected_token = "|";
                break;
            }
             msh->last_token = token_add(&msh->tokens, TK_PIPE, NULL, NULL);
             input++;
        }
        else if (is_shell_space(*input))
            input++;
        else
        {
            word = read_word(&input);
            if (word)
                msh->last_token = token_add(&msh->tokens, TK_WORD, word, NULL);
            else
            {   // TODO: there was a problem reading the word...
                msh->err_syntax = true;
                msh->unexpected_token = "newline";
                break;
            }
        }
    }
    print_tokens(&msh->tokens);
    if (msh->last_token && msh->last_token->tk_type == TK_PIPE)
        msh->mult_line_input = true;
    else
        msh->mult_line_input = false;
    return (SUCCESS);
}
