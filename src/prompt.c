/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:49:31 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 21:21:33 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_relative_dir(t_string *prompt, char *cwd, const char *home)
{
	const int	len_left = PROMPT_MAX_LEN - ft_strlen(PROMPT_INVITATION)
		- ft_strlen(FT_COL_MAGENTA) - ft_strlen(FT_COL_DEFAULT);
	const int	cont_len = ft_strlen(cwd);
	const int	home_len = ft_strlen(home);

	if (home_len && ft_strncmp(home, cwd, home_len) == SUCCESS)
	{
		if (cont_len - home_len + 1 < len_left)
			(string_add_str(prompt, "~"),
				string_add_str(prompt, cwd + home_len));
		else
			(string_add_str(prompt, "..."),
				string_add_str(prompt, cwd + cont_len - len_left + 3));
	}
	else if (cont_len > len_left)
		(string_add_str(prompt, "..."),
			string_add_str(prompt, cwd + cont_len - len_left + 3));
	else
		string_add_str(prompt, cwd);
}

void	update_prompt(t_msh *msh)
{
	const char		*home = varlist_get_value(msh->env, "HOME");
	const t_var		*pwd_v = varlist_find(msh->env, "PWD");
	char			*cwd;

	msh->prompt.buf[0] = '\0';
	msh->prompt.len = 0;
	string_add_str(&msh->prompt, FT_COL_MAGENTA);
	if (pwd_v)
		add_relative_dir(&msh->prompt, pwd_v->value, home);
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			add_relative_dir(&msh->prompt, "cannot update prompt", "");
		else
			(add_relative_dir(&msh->prompt, cwd, home), free(cwd));
	}
	string_add_str(&msh->prompt, FT_COL_DEFAULT);
	string_add_str(&msh->prompt, PROMPT_INVITATION);
}
