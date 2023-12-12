#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"

typedef struct s_minish
{
	char	pwd[PATH_MAX];
	char	*prompt;
	char	**envp;

}			t_minish;

void	ms_init(t_minish *minish);
void	ms_update(t_minish *minish);
void	ms_error_exit(t_minish *minish, char *err_msg);


#endif
