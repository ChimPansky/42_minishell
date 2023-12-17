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

## signature of pipex:
" abc"
