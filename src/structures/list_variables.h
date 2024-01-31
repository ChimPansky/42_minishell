#ifndef VARIABLES_H
#define VARIABLES_H

#include "libft.h"

typedef struct s_var
{
	char *name;
	char *value;
}		t_var;

typedef t_list t_variables;

char		*var_get_value(t_variables *vars, const char *name);
t_var		*var_find(t_variables *vars, const char *name);
t_var		*var_set(t_variables **vars_p, const char *name, const char *value);
void		var_delete(t_variables **vars_p, const char *name);
void		vars_destoy(t_variables **vars_p);

t_variables	*vars_init_from_envp(char **envp);
// on fail returns null
char		**vars_convert_to_array(t_variables *vars);

#endif  // VARIABLES_H
