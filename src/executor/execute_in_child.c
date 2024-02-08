#include "executor.h"
#include "ft_charptr_array.h"
#include <sys/stat.h>

// fileno in child process always STDOUT_FILENO
static int try_exec_built_in(t_msh *msh, char **cmd_with_args)
{
	const t_built_in func = get_built_in_by_name(cmd_with_args[0]);

	if (func == NULL)
		return (!SUCCESS);
	msh->last_exit_code = func(msh, cmd_with_args, STDOUT_FILENO);
	return (SUCCESS);
}

bool	file_found_on_path(const char *exec_in_path)
{
    struct stat fstat;

	if (access(exec_in_path, F_OK) == SUCCESS)
		return (false);
	if (SUCCESS != stat(exec_in_path, &fstat))
		return (false);
    return (S_ISREG(fstat.st_mode));
}

typedef struct s_child
{
	const char	*path;
	const char	*exec;
	char		*exec_with_path;
}		t_child;

static int	try_find_in_path(t_child *ch)
{
	char 		**path_entries;
	size_t 		i;

	path_entries = ft_split(ch->path, ":");
	if (path_entries == NULL)
		return (perror("ft_split"), !SUCCESS);
	i = 0;
	while (path_entries[i])
	{
		if (ft_str_end_with(ch->exec_with_path, "/"))
			ch->exec_with_path = ft_strnjoin(2, path_entries[i], ch->exec);
		else
			ch->exec_with_path = ft_strnjoin(3, path_entries[i], "/", ch->exec);
		if (!ch->exec_with_path)
			return (free(path_entries), perror("ft_strnjoin"), !SUCCESS);
		if (file_found_on_path(ch->exec_with_path))
			return (free(path_entries), SUCCESS);
		free(ch->exec_with_path);
		i++;
	}
	ch->exec_with_path = NULL;
	return (free(path_entries), SUCCESS);
}

int	init_child(t_msh *msh, t_child *ch, char **cmd_with_args)
{
	ch->path = varlist_get_value(msh->env, "PATH");
	ch->exec = cmd_with_args[0];
	ch->exec_with_path = NULL;
	if (NULL == strchr(ch->exec, '/') && *ch->path)
	{
		if (try_find_in_path(ch) != SUCCESS)
		 	return (!SUCCESS);
	}
	else
	{
		ch->exec_with_path = ft_strdup(ch->exec);
		if (ch->exec_with_path)
			return (perror("execute_in_child_process: ft_strdup"), !SUCCESS);
	}
	return (SUCCESS);
}

void	destroy_child(t_child *ch)
{
	free(ch->exec_with_path);
}

static int check_permissions(t_msh *msh, const char* exec)
{
    struct stat fstat;
	if (exec == NULL)
	{
		msh->last_exit_code = EXIT_COMMAND_NOT_FOUND;
		return (ft_printf_err("msh: command not found: %s\n", exec), !SUCCESS);
	}
	else if (SUCCESS != access(exec, F_OK))
	{
		msh->last_exit_code = EXIT_COMMAND_NOT_FOUND;
		return (ft_printf_err("msh: %s: no such file or directory\n", exec), !SUCCESS);
	}
    if (SUCCESS != stat(exec, &fstat))
		return (perror("check_permissions: stat"), !SUCCESS);
    if (!S_ISDIR(fstat.st_mode))
	{
		msh->last_exit_code = EXIT_PERMISSION_DENIED;
		return (ft_printf_err("msh: %s: is a directory\n", exec), !SUCCESS);
	}
	if (SUCCESS != access(exec, X_OK))
	{
		msh->last_exit_code = EXIT_PERMISSION_DENIED;
		return (ft_printf_err("msh: %s: permission denied\n", exec), !SUCCESS);
	}
	return (SUCCESS);
}

// exit code is set to EXIT_FAILURE higher on stack
// return value may be skipped
int	execute_in_child_process(t_msh *msh, char **cmd_with_args)
{
	t_charptr_array		envp;
	t_child				ch;

	if (NULL == strchr(ch.exec, '/')
		&& try_exec_built_in(msh, cmd_with_args) == SUCCESS)
		return (SUCCESS);
	if (SUCCESS != init_child(msh, &ch, cmd_with_args))
		return (!SUCCESS);
	if (check_permissions(msh, ch.exec) != SUCCESS)
		return (destroy_child(&ch), SUCCESS);
	if (varlist_convert_to_array(msh->env, &envp) != SUCCESS)
		return (perror("execute_in_child_process: "
				"varlist_convert_to_array"), destroy_child(&ch), !SUCCESS);
	execve(ch.exec_with_path, cmd_with_args, envp.buf);
	return (perror("execute_in_child_process: exeve"),
			charptr_array_destroy(&envp), destroy_child(&ch), !SUCCESS);
}
