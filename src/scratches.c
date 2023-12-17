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
// 		ft_printf_fd(msh->out_fd, "%s\n", cmd_with_args[i]);
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
