#include "minishell.h"
#include "libft.h"

t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						t_redir_type fd_type, char *str)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	ft_bzero(token, sizeof(token));
	token->tk_type = tk_type;
	token->fd_type = fd_type;
	if (str)
	{
		token->str = ft_strdup(str);
		if (!token->str)
			return (free(token), NULL);
	}
	else
		token->str = NULL;
	t_tokens *new_token = ft_lstnew(token);
	if (!new_token)
		return (destroy_token(token), NULL);
	ft_lstadd_back(tokens, new_token);
	return (token);
}

void	destroy_token(void *token_void)
{
	t_token *token = token_void;

	if (token->str)
		free(token->str);
	ft_lstclear(&token->subshell, destroy_token);
	free(token);
}

void	print_tokens(t_tokens **tokens)
{
	t_tokens	*cur_list;
	t_token		*cur_token;
	int			i;

	if (!tokens)
		return ;
	cur_list = *tokens;
	i = 0;
	while (cur_list && cur_list->content)
	{
		cur_token = cur_list->content;
		printf("%p <--(Token Nr. %d: Type: %d; Str: %s)-->%p", cur_list->prev, i, cur_token->tk_type, cur_token->str, cur_list->next);
		// ft_putstr_fd("Token Nr.", 1);
		// ft_putnbr_fd(i, 1);
		// ft_putstr_fd(": Type: ", 1);
		// ft_putnbr_fd(cur_token->type, 1);
		// ft_putstr_fd("; Str: ", 1);
		// if (cur_token->str)
		// 	ft_putendl_fd(cur_token->str, 1);
		// else
		// 	ft_putendl_fd("(null)", 1);
		cur_list = cur_list->next;
		i++;
	}
}

// t_var *variable(const char *name, const char *value)
// {
// 	t_var		*var;
// 	const char	*default_value = "";

// 	var = malloc(sizeof(t_var));
// 	if (!var)
// 		return NULL;
// 	var->name = ft_strdup(name);
// 	if (!var->name)
// 		return (free(var), NULL);
// 	if (!value)
// 		value = default_value;
// 	var->value = ft_strdup(value);
// 	if (!var->value)
// 		return (free(var->name), free(var), NULL);
// 	return var;
// }

// void destroy_variable(void *var_void)
// {
// 	t_var *var = var_void;

// 	free(var->name);
// 	free(var->value);
// 	free(var);
// }

// char	*var_get_value(t_variables *vars, const char *name)
// {
// 	t_var	*target;

// 	target = var_find(vars, name);
// 	if (!target)
// 		return (NULL);
// 	return (target->value);
// }

// t_var *var_find(t_variables *vars, const char *name)
// {
// 	while (vars)
// 	{
// 		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
// 			return vars->content;
// 		vars = vars->next;
// 	}
// 	return NULL;
// }
// t_var	*var_set(t_variables **vars_p, const char *name, const char *value)
// {
// 	t_var	*existing_var;
// 	char	*new_value;

// 	if ((existing_var = var_find(*vars_p, name)) != NULL)
// 	{
// 		new_value = ft_strdup(value);
// 		if (!new_value)
// 			return (NULL);
// 		free(existing_var->value);
// 		existing_var->value = new_value;
// 		return (existing_var);
// 	}
// 	else
// 		return (var_add(vars_p, name, value));
// }

// t_var *var_add(t_variables **vars_p, const char *name, const char *value)
// {
// 	t_var *var = variable(name, value);
// 	if (!var)
// 		return NULL;
// 	t_variables *new_var = ft_lstnew(var);
// 	if (!new_var)
// 		return (destroy_variable(var), NULL);
// 	ft_lstadd_front(vars_p, new_var);
// 	return var;
// }

// void var_delete(t_variables **vars_p, const char *name)
// {
// 	t_variables *vars;

// 	vars = *vars_p;
// 	while (vars)
// 	{
// 		if (ft_strcmp(((t_var*)vars->content)->name, name) == SUCCESS)
// 		{
// 			ft_lstdel_node(vars_p, vars, destroy_variable);
// 			return ;
// 		}
// 		vars = vars->next;
// 	}
// }

// t_variables *vars_init_from_envp(char **envp)
// {
// 	t_variables	*env;
// 	char	*sep;

// 	env = NULL;
// 	if (!envp)
// 		return NULL;
// 	while (*envp)
// 	{
// 		sep = ft_strchr(*envp, '=');
// 		*sep = 0;
// 		if (NULL == var_add(&env, *envp, sep + 1))
// 			return (ft_lstclear(&env, destroy_variable), NULL);
// 		envp++;
// 	}
// 	return env;
// }

// // on fail returns null
// char **vars_convert_to_array(t_variables *vars)
// {
// 	char	**vars_array;
// 	size_t	i;

// 	vars_array = ft_calloc(ft_lstsize(vars) + 1, sizeof(char *));
// 	if (!vars_array)
// 		return NULL;
// 	i = 0;
// 	while (vars)
// 	{
// 		vars_array[i] = ft_strnjoin(3, ((t_var*)vars->content)->name, "=", ((t_var*)vars->content)->value);
// 		if (!vars_array[i])
// 		{
// 			while (i--)
// 				free(vars_array[i]);
// 			return (free(vars_array), NULL);
// 		}
// 		vars = vars->next;
// 		i++;
// 	}
// 	return vars_array;
// }
