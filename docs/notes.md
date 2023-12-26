# minishell

string library

string replace
is empty
char
edit ft_split: separator is char * instead of char
exit export cd echo pwd


## signature of built-in functions

exit with correct error code, perror if one happened
output to msh->out_fd, err to msh->err_fd
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
