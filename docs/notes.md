# minishell

## errors convention:
if error before main loop: exit with reason in any format
if error should not happen (assert): `ft_printf_err("CRIT ERR: description")`
if system error (malloc, open): `perror("context_function: failed_function)`
if bash error (ambiguous redir, syntax err): `ft_printf_err("msh: part: reason")`

## signature of built-in functions
exit with correct error code, perror if one happened
output to msh->fd_out, err to msh->err_fd
cmd_with_args[0] == built-in func name, e.g. "pwd"

