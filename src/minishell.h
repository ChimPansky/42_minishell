#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"

#define EXIT_COMMAND_NOT_FOUND		127
#define  EXIT_PERMISSION_DENIED		126

typedef struct s_var
{
	char *name;
	char *value;
}	t_var;

typedef t_list t_variables;

typedef struct s_msh
{
	char		pwd[PATH_MAX];
	char		*prompt;
	int			in_fd;
	int			out_fd;
	int			err_fd;
	int			last_exit_code;
	int			pid_to_wait;
	t_variables *env;
	t_variables	*locals;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

t_built_in get_built_in_by_name(char *func_name);

void	ms_init(t_msh *minish, char **envp);
void	ms_update(t_msh *minish);
void	ms_error_exit(t_msh *minish, char *err_msg);

void execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments);
int parse(t_msh *msh, char *input);

char *find_env(t_msh *msh, const char *var_name);
bool is_empty(const char *str);

t_var *var_find(t_variables *vars, const char *name);
t_var *var_add(t_variables **vars_p, const char *name, const char *value);
void var_delete(t_variables **vars_p, const char *name);
t_variables *vars_init_from_envp(char **envp);
char **vars_convert_to_array(t_variables *vars);

#endif
