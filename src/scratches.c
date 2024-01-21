#include "minishell.h"

// void	handle_input(t_msh *minish, char *input)
// {
// 	if (ft_strncmp(input, "pwd", ft_strlen("pwd")) == 0)
// 		printf("%s\n", getcwd(minish->pwd, PATH_MAX));
// }


// int built_ex(t_msh *msh, char **cmd_with_args)
// {
// 	for (int i = 0; cmd_with_args[i] != NULL; i++)
// 	{
// 		ft_printf_fd(msh->fd_out, "%s\n", cmd_with_args[i]);
// 	}
// 	return 0;
// }

// t_built_in get_built_in_by_name(char *func_name)
// {
// 	if (strncmp(func_name, "built_ex", 9) == 0) {
// 		return built_ex;
// 	}
// 	return NULL;
// }
// char	*var_get_value(t_variables *vars, const char *name)
// {
// 	t_var	*target;

// 	target = var_find(vars, name);
// 	if (!target)
// 		return (NULL);
// 	return (target->value);
// }

char *find_env(t_msh *msh, const char *var_name)
{
	(void) msh;
	(void) var_name;
	return "/bin:/usr/bin";
}

bool is_empty(const char *str)
{
	return (str != NULL && *str == '\0');
}

void	print_splitted(char **splitted)
{
	int	i;

	i = 0;
	if (!splitted)
	{
		ft_putstr_fd("NULL\n", 1);
		return ;
	}
	while (splitted && splitted[i])
	{
		ft_putstr_fd("char[",1);
		ft_putnbr_fd(i, 1);
		ft_putstr_fd("]: ", 1);
		ft_putstr_fd(splitted[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("char[",1);
	ft_putnbr_fd(i, 1);
	ft_putstr_fd("]: ", 1);
	ft_putstr_fd("NULL\n", 1);
}
