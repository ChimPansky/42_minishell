#include "minishell.h"
#include "libft.h"

t_var *variable(const char *name, const char *value)
{
	t_var		*var;
	const char	*default_value = "";

	var = malloc(sizeof(t_var));
	if (!var)
		return NULL;
	var->name = ft_strdup(name);
	if (!var->name)
		return (free(var), NULL);
	if (!value)
		value = default_value;
	var->value = ft_strdup(value);
	if (!var->value)
		return (free(var->name), free(var), NULL);
	return var;
}

void destroy_variable(void *var_void)
{
	t_var *var = var_void;

	free(var->name);
	free(var->value);
	free(var);
}

char	*var_get_value(t_variables *vars, const char *name)
{
	t_var	*target;
	
	target = var_find(vars, name);
	if (!target)
		return ("");
	return (target->value);
}

t_var *var_find(t_variables *vars, const char *name)
{
	while (vars)
	{
		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
			return vars->content;
		vars = vars->next;
	}
	return NULL;
}
t_var	*var_set(t_variables **vars_p, const char *name, const char *value)
{
	t_var	*existing_var;
	char	*new_value;

	if ((existing_var = var_find(*vars_p, name)) != NULL)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (NULL);
		free(existing_var->value);
		existing_var->value = new_value;
		return (existing_var);
	}
	else
		return (var_add(vars_p, name, value));
}

t_var *var_add(t_variables **vars_p, const char *name, const char *value)
{
	t_var *var = variable(name, value);
	if (!var)
		return NULL;
	t_variables *new_var = ft_lstnew(var);
	if (!new_var)
		return (destroy_variable(var), NULL);
	ft_lstadd_front(vars_p, new_var);
	return var;
}

void var_delete(t_variables **vars_p, const char *name)
{
	t_variables *vars;

	vars = *vars_p;
	while (vars)
	{
		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
		{
			ft_lstdel_node(vars_p, vars, destroy_variable);
			return ;
		}
		vars = vars->next;
	}
}

t_variables *vars_init_from_envp(char **envp)
{
	t_variables	*env;
	char	*sep;

	env = NULL;
	if (!envp)
		return NULL;
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		*sep = 0;
		if (NULL == var_add(&env, *envp, sep + 1))
			return (ft_lstclear(&env, destroy_variable), NULL);
		envp++;
	}
	return env;
}

// on fail returns null
char **vars_convert_to_array(t_variables *vars)
{
	char	**vars_array;
	size_t	i;

	vars_array = ft_calloc(ft_lstsize(vars) + 1, sizeof(char *));
	if (!vars_array)
		return NULL;
	i = 0;
	while (vars)
	{
		vars_array[i] = ft_strnjoin(3, ((t_var*)vars->content)->name, "=", ((t_var*)vars->content)->value);
		if (!vars_array[i])
		{
			while (i--)
				free(vars_array[i]);
			return (free(vars_array), NULL);
		}
		vars = vars->next;
		i++;
	}
	return vars_array;
}
