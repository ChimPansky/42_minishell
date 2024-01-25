# minishell

string library

string replace
is empty
char
edit ft_split: separator is char * instead of char
exit export cd echo pwd


## signature of built-in functions

exit with correct error code, perror if one happened
output to msh->fd_out, err to msh->err_fd
cmd_with_args[0] == built-in func name, e.g. "pwd"

```
int name(t_msh *msh, char **cmd_with_args);
```
if subject says "implement env without options or arguments":
	shall we ignore all arguments and options and just mimic "env" or output an error:
	"invalid argument/option" / "no arguments/options allowed"...

## signature of pipex:
" abc"

# env variables:
in init: create copy of env and store in local? check format of output of "export" in bash...

validate variablenames (check for invalid characters). e.g. VAR.1 is not a valid name...
=.!;:


# TODO Tom:
Syntax check of whole input (from readline)
Tokenizer: convert whole input into tokens (string:redir_in, pipe:NULL, OB, CB, string:redir_out, ...)

# Malloc Errors:
find a nice way to handle them, without exiting the whole minishell.
when a malloc error in a random function occurs while lexing/parsing/executing/... -> reset and give control back to user (show msh prompt)

# Expression in braces:
lets create another builtin function "execute_in_subshell with usual signature"
when lexer finds open brace it creates a token and put content in between braces inside
when parser turns token to cmd with args with a special executable name
another option is an actual executor on tree, not on chain
heredoc will be a problem without trees. mb dump to file and replace with file name
