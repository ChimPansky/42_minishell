#include "../minishell.h"

char	*get_last_exit_code(t_msh *msh)
{
	static char	ret_code_buf[4];
	int			pos;

	pos = 0;
	if (msh->last_exit_code > 100)
		ret_code_buf[pos++] = '0' + msh->last_exit_code / 100 % 10;
	if (msh->last_exit_code > 10)
		ret_code_buf[pos++] = '0' + msh->last_exit_code / 10 % 10;
	ret_code_buf[pos++] = '0' + msh->last_exit_code % 10;
	ret_code_buf[pos] = '\0';
	return ret_code_buf;
}
