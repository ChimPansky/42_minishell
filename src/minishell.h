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
	char	**envp; // will be read from main envp argument and stored in linked list, so we can add and remove and modifvy env variables
	int		in_fd;
	int		out_fd;
	int		err_fd;
	int		last_exit_code;
	int		pid_to_wait;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

// init.c
void	init(t_msh *msh);

// update.c
void	update_pwd(t_msh *msh);
void	update_prompt(t_msh *msh);
void	update(t_msh *msh);

//exit_error.c
void	ms_exit(t_msh *msh, int error_nr);
void	ms_error(int error_nr);

// scratches.c
char 	*find_env(t_msh *msh, const char *var_name);
bool 	is_empty(const char *str);

// pipex.c
void 	execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments);

// built_ins/built_in.c
t_built_in get_built_in_by_name(char *func_name);

// parser.c
int 	parse(t_msh *msh, char *input);

// destroy.c
void	destroy(t_msh *msh);

#endif
