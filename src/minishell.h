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
	char	pwd[PATH_MAX];
	char	*prompt;
	char	**envp;
	int		in_fd;
	int		out_fd;
	int		err_fd;
	int		last_exit_code;
	int		pid_to_wait;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

t_built_in get_built_in_by_name(char *func_name);

void	ms_init(t_msh *minish);
void	ms_update(t_msh *minish);
void	ms_error_exit(t_msh *minish, char *err_msg);

void execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments);
int parse(t_msh *msh, char *input);

char *find_env(t_msh *msh, const char *var_name);
bool is_empty(const char *str);

#endif
