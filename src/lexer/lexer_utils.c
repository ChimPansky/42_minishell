/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 18:36:01 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

size_t read_shell_spaces(char **input)
{
	size_t  sep_count;

	sep_count = 0;
	while (ft_isspace(**input))
	{
		*(input) += 1;
		sep_count++;
	}
	return (sep_count);
}

static int read_word(t_msh *msh, char **pos_in_input, t_string *str)
{
	char    quote_type;

	quote_type = 0;
	read_shell_spaces(pos_in_input);
	while (**pos_in_input)
	{
		if (quote_type == 0 && is_token_seperator(**pos_in_input))
			break;
		if (quote_type == 0
			&& (**pos_in_input == '\'' || **pos_in_input == '"'))
			quote_type = **pos_in_input;
		else if (**pos_in_input == quote_type)
			quote_type = 0;
		string_add_chr(str, **pos_in_input);
		(*pos_in_input) += 1;
	}
	if (quote_type != 0 || string_is_empty(str))
	{
		error_unexpected_token(**pos_in_input);
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (!SUCCESS);
	}
	return (SUCCESS);
}

static	t_redir_type	read_redir_type(char **pos_in_input)
{
	if (ft_strncmp("<<", *pos_in_input, 2) == SUCCESS)
	{
		*pos_in_input += 2;
		return (FD_HEREDOC);
	}
	else 	if (ft_strncmp(">>", *pos_in_input, 2) == SUCCESS)
	{
		*pos_in_input += 2;
		return (FD_OUT_APPEND);
	}
	else if (**pos_in_input == '<')
	{
		*pos_in_input += 1;
		return (FD_IN);
	}
	else if (**pos_in_input == '>')
	{
		*pos_in_input += 1;
		return (FD_OUT_TRUNC);
	}
	return (FD_NULL);
}

int  read_tk_redir(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
				t_token_type *last_tk_type)
{
	t_token			*new_token;
	t_redir_detail	*new_redir;

	new_token = tokenlist_add_token(tokens_p, TK_REDIR);
	if (!new_token)
		return (!SUCCESS);
	if (redir_init(&new_redir, read_redir_type(pos_in_input)) != SUCCESS)
		return (!SUCCESS);
	if (read_word(msh, pos_in_input, &(new_redir->string)) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	new_token->redir = new_redir;
	*last_tk_type = TK_REDIR;
	return (SUCCESS);
}

static	t_token_type	read_tk_simple_cmd_separator_type(char **pos_in_input)
{
	if (ft_strncmp("&&", *pos_in_input, 2) == SUCCESS)
	{
		*pos_in_input += 2;
		return (TK_LOGIC_AND);
	}
	else if (ft_strncmp("||", *pos_in_input, 2) == SUCCESS)
	{
		*pos_in_input += 2;
		return (TK_LOGIC_OR);
	}
	else if (**pos_in_input == '|')
	{
		*pos_in_input += 1;
		return (TK_PIPE);
	}
	return (TK_NULL);
}

int	read_tk_simple_cmd_separator(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
t_token_type *last_tk_type)
{
	t_token_type	tk_type;

	if (*last_tk_type == TK_NULL)
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (error_unexpected_token(**pos_in_input), !SUCCESS);
	}
	tk_type = read_tk_simple_cmd_separator_type(pos_in_input);
	if (!tokenlist_add_token(tokens_p, tk_type))
		return (!SUCCESS);
	*last_tk_type = tk_type;
	read_shell_spaces(pos_in_input);
	if (!**pos_in_input || **pos_in_input == '&' || **pos_in_input == '|')
	{
		msh->last_exit_code = ER_UNEXPECTED_TOKEN;
		return (error_unexpected_token(**pos_in_input), !SUCCESS);
	}
	return (SUCCESS);
}

int	read_tk_word(t_msh *msh, t_tokenlist **tokens_p, char **pos_in_input,
				t_token_type *last_tk_type)
{
	t_token			*new_token;

	new_token = tokenlist_add_token(tokens_p, TK_WORD);
	if (!new_token)
		return (!SUCCESS);
	if (read_word(msh, pos_in_input, &new_token->string) != SUCCESS)
		return (!SUCCESS);
	*last_tk_type = TK_WORD;
	return (SUCCESS);
}

// int  read_redir(t_redir_detail *redir, char **input)
// {
// 	if (ft_strncmp("<<", *input, 2) == SUCCESS)
// 		redir->type = FD_HEREDOC;
// 	else if (ft_strncmp(">>", *input, 2) == SUCCESS)
// 		redir->type = FD_OUT_APPEND;
// 	else if (**input == '<')
// 		redir->type = FD_IN;
// 	else if (**input == '>')
// 		redir->type = FD_OUT_TRUNC;
// 	if (redir->type == FD_HEREDOC || redir->type == FD_OUT_APPEND)
// 		(*input) += 2;
// 	else
// 		(*input) += 1;
// 	if (read_word(input, &redir->string) != SUCCESS)
// 		return (!SUCCESS);
// 	return (SUCCESS);
// }
