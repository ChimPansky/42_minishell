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

char	*get_pwd(t_msh *msh)
{
	const t_var*	pwd_v = var_find(msh->env, "PWD");
	char			*pwd;

	if (pwd_v)
		pwd = ft_strdup(pwd_v->value);
	else
		pwd = getcwd(NULL, 0);
	if (!pwd)
		return NULL;

}

void	update_prompt(t_msh *msh)
{
	const char*		home = var_get_value(msh->env, "HOME");

	const int prompt_cont_max_len = PROMPT_MAX_LEN
		- ft_strlen(FT_COL_MAGENTA) - ft_strlen(FT_COL_DEFAULT) - 3;
	ft_strlcpy(msh->prompt, const char *src, size_t size)
	msh->prompt = ft_strnjoin(4, FT_COL_MAGENTA, pwd, FT_COL_DEFAULT, "$ ");
	if (!msh->prompt)
		msh->prompt = ft_strnjoin(4, FT_COL_MAGENTA, "unknown pwd", FT_COL_DEFAULT, "$ ");
}

void	update(t_msh *msh)
{
	(void)msh;
	//update_pwd(msh);
	//update_prompt(msh);
}
