/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:18:59 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/04 17:31:51 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../minishell.h"

typedef struct s_parser
{
	t_cmdlist			*cmdlist;
	t_simple_command	*cmd;
	t_tokenlist			*tokenlist;
	t_token				*token;
	t_token_type		tk_type;
	bool				done;
}		t_parser;

int		parse_redirection(t_msh *msh, t_parser *parser);
int		parse_word(t_msh *msh, t_parser *parser);
int		parse_subshell(t_parser *parser);
int		parse_separators(t_msh *msh, t_parser *parser);

typedef struct s_expander
{
	const char		*pos;
	t_list			*true_wildcards;
	t_string		replace;
	bool			add_if_empty;
	bool			glob;
}		t_expander;

int		expand_heredoc(t_msh *msh, t_charptr_array *heredoc_lines);
int		check_for_wc_and_improve(t_expander *expander, const char **str);
int		expand_string_to_arr(
			t_msh *msh,
			char *string,
			t_charptr_array *str_array);
int		expand_singleq(t_expander *expander);
int		expand_variable(t_msh *msh, t_expander *expander, t_charptr_array *arr);
char	*get_var_content(t_msh *msh, const char **pos_in_str);
int		expand_wildcard_and_finalize(
			t_expander *expander,
			t_charptr_array *arr);

#endif  // PARSER__H
