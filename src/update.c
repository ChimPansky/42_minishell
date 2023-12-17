/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:49:31 by tkasbari          #+#    #+#             */
/*   Updated: 2023/12/15 15:33:24 by tkasbari         ###   ########.fr       */
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
		ms_error(0);
	}
}

void	update_prompt(t_msh *msh)
{
	if (msh->pwd[0])
		msh->prompt = ft_strjoin(msh->pwd, ": ");
	else
		msh->prompt = ft_strjoin("", ": ");
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
