#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"

typedef struct s_minish
{
	char	*rl_input;
	char	*prompt;
	char	pwd[PATH_MAX + 1];
	char	**envp;

}			t_minish;

// ms_init.c
void	ms_init(t_minish *minish);
// ms_update.c
void	ms_update_pwd(t_minish *minish);
void	ms_update(t_minish *minish);
//ms_exit_error.c
void	ms_exit(t_minish *minish, int error_nr);
// ms_builtin_cmds.c
int		ms_builtin_pwd(t_minish *minish, char **cmd_with_args);
int		ms_builtin_exit(t_minish *minish, char **cmd_with_args);
// ms_destroy.c
void	ms_destroy(t_minish *minish);


#endif
