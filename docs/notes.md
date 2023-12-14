# minishell

```
typedef struct s_minishell
{
	env
} t_msh

```
## signature of built-in functions

function should exit with correct return code
and err msg if one occurs;

cmd_with_args[0] == built-in func name, e.g. "pwd"

```
void name(t_msh *msh, char **cmd_with_args);
```

## signature of pipex:
