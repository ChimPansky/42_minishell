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

typedef struct s_minish
{
	char	pwd[PATH_MAX];
	char	*prompt;
	char	**envp;

}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

void	ms_init(t_msh *minish);
void	ms_update(t_msh *minish);
void	ms_error_exit(t_msh *minish, char *err_msg);


#endif
