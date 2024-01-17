/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:49:31 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/14 14:11:32 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

void	update_pwd(t_msh *msh)
{
	if (!getcwd(msh->pwd, PATH_MAX))
	{
		ft_strlcpy(msh->pwd, "Unknown", ft_strlen("Unknown") + 1);
		errno = 1;
		//perror("pwd: error retrieving current working directory: getcwd");
		ms_error(ER_UNDEFINED);
	}
}

void	update_prompt(t_msh *msh)
{
	msh->prompt = ft_strnjoin(4, FT_COL_MAGENTA, msh->pwd, FT_COL_DEFAULT, "$ ");
	if (!msh->prompt)
		ms_exit(msh, EXIT_FAILURE);
}
// void set_env_var(t_msh *msh, char *var_name);
// void get_env_var();
void	update(t_msh *msh)
{
	(void)msh;
	//update_pwd(msh);
	//update_prompt(msh);
}
