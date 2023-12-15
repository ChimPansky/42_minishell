#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"

#define SUCCESS 0

#define EXIT_COMMAND_NOT_FOUND		127
#define  EXIT_PERMISSION_DENIED		126

typedef struct s_msh
{
	char	*rl_input;
	char	*prompt;
	char	pwd[PATH_MAX + 1];
	char	**envp;
	int		in_fd;
	int		out_fd;
	int		err_fd;
	int		last_exit_code;
	int		pid_to_wait;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

// ms_init.c
void	ms_init(t_msh *msh);

// ms_update.c
void	ms_update_pwd(t_msh *msh);
void	ms_update(t_msh *msh);

//ms_exit_error.c
void	ms_exit(t_msh *msh, int error_nr);

// ms_builtin_cmds.c
int		ms_builtin_pwd(t_msh *msh, char **cmd_with_args);
int		ms_builtin_exit(t_msh *msh, char **cmd_with_args);

// scratches.c
t_built_in get_built_in_by_name(char *func_name);
char 	*find_env(t_msh *msh, const char *var_name);
bool 	is_empty(const char *str);

// pipex.c
void 	execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments);

// parser.c
int 	parse(t_msh *msh, char *input);

// ms_destroy.c
void	ms_destroy(t_msh *msh);

#endif
