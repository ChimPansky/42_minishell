#include "minishell.h"
#include <sys/wait.h>

// if brace, look for the other brace, execute in subshell
// if && || or ; execute everything before
// if pipe in expression open pipe first, redirect output to pipe-in-fd execute before pipe
// if pipe before expression redirect input from pipe-out-fd
// if < > << >> remove from expression + redirect. on redirect close previous
// if no pipe in expression check for builtins first, then fork
// work with braces


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

    (void) in_squotes;
    (void) in_dquotes;
    (void) in_brackets;
    (void) in_word;
    (void) start;
    (void) word;
    // grep NAME < Makefile > out1.txt |
    token_add(&tokens, TK_WORD, 0, "grep");
    token_add(&tokens, TK_WORD, 0, "NAME");
    token_add(&tokens, TK_REDIR, FD_IN, "Makefile");
    token_add(&tokens, TK_REDIR, FD_OUT_TRUNC, "out1.txt");
    token_add(&tokens, TK_PIPE, 0, NULL);

    while (*input)
    {
        // if ((in_squotes && *input == '\'') || (in_dquotes && *input == '"') || (in_word && is_word_sep(*input)))
        // {
        //     in_squotes = false;
        //     in_dquotes = false;
        //     in_word = false;
        //     word = ft_substr(input, start, i - start);
        //     if (!word)
        //         ms_exit(msh, ERROR);    // TODO ERROR
        //     token_add(&tokens, TK_WORD, 0, word);
        // }
        // else if (!in_squotes && !in_dquotes && !in_word && (*input == '\'' || *input == '"'))
        // {
        //     start = i + 1;
        //     if (*input == '\'')
        //         in_squotes = true;
        //     else
        //         in_dquotes = true;
        // }
        // if (*input == '=')
        //     token_add(&tokens, TK_ASSIGNMENT, 0, NULL);
        // if (*input == '<' || *input == '>')
        //     token_add_redir(msh, tokens, &input, &i)
        // if (*input == '|')
        //     token_add(&msh->tokens, TK_PIPE, NULL);
        // // BONUS:
        // if (*input == '*')
        //     token_add(&msh->tokens, TK_WILDCARD, NULL);
        // if (*input == '(')
        //     token_add(&msh->tokens, TK_BRACKET_OPEN, NULL);
        // if (*input == ')')
        //     token_add(&msh->tokens, TK_BRACKET_CLOSE, NULL);
        // if (strcmp(input, "&&") == SUCCESS)
        // {
        //     token_add(&msh->tokens, TK_LOGIC_AND, NULL);
        //     input += 1;
        // }
        // if (strcmp(input, "||") == SUCCESS)
        // {
        //     token_add(&msh->tokens, TK_LOGIC_OR, NULL);
        //     input += 1;
        // }
        input++;
        i++;
    }
    print_tokens(&msh->tokens);
    return (0);
}
