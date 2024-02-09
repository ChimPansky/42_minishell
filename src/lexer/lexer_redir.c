/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:21:43 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "ft_charptr_array.h"
#include "lexer.h"

static int	check_ctrl_d(char *line, char *delimiter)
{
	if (line == NULL)
	{
		if (errno)
			return (perror("readline heredoc"), !SUCCESS);
		ft_printf("\nwarning: here-document delimited "
			"by end-of-file (wanted '%s')\n", delimiter);
	}
	return (SUCCESS);
}

static int	lex_heredoc(t_msh *msh, t_redir_detail *redir)
{
	char		*line;
	t_string	line_w_nl;

	if (!(ft_strlen(redir->string.buf) > string_remove_quotes(&redir->string)))
		redir->expand_heredoc = true;
	while (1)
	{
		line = readline_wrapper(PROMPT_HEREDOC, true);
		if (check_for_signals(msh))
			return (free(line), !SUCCESS);
		if (check_ctrl_d(line, redir->string.buf) != SUCCESS)
			return (!SUCCESS);
		if (line == NULL || ft_strcmp(redir->string.buf, line) == SUCCESS)
			break ;
		string_init_with_allocated(&line_w_nl, line);
		if (string_add_chr(&line_w_nl, '\n') != SUCCESS)
			return (string_destroy(&line_w_nl), !SUCCESS);
		if (charptr_array_add_allocated_str(&redir->content,
				&(line_w_nl.buf)) != SUCCESS)
			return (!SUCCESS);
	}
	return (free(line), SUCCESS);
}

static void	read_redir_type(t_lexer *lexer)
{
	if (ft_strncmp("<<", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->redir_type = FD_HEREDOC;
	}
	else if (ft_strncmp(">>", lexer->pos_in_input, 2) == SUCCESS)
	{
		lexer->pos_in_input += 2;
		lexer->redir_type = FD_OUT_APPEND;
	}
	else if (*lexer->pos_in_input == '<')
	{
		lexer->pos_in_input += 1;
		lexer->redir_type = FD_IN;
	}
	else if (*lexer->pos_in_input == '>')
	{
		lexer->pos_in_input += 1;
		lexer->redir_type = FD_OUT_TRUNC;
	}
	else
		lexer->redir_type = FD_NULL;
}

int	lex_tk_redir(t_msh *msh, t_lexer *lexer)
{
	t_token			*new_token;
	t_redir_detail	*new_redir;

	new_token = tokenlist_add_token(lexer->tokens, TK_REDIR);
	if (!new_token)
		return (!SUCCESS);
	read_redir_type(lexer);
	if (redir_init(&new_redir, lexer->redir_type) != SUCCESS)
		return (!SUCCESS);
	read_shell_spaces(&lexer->pos_in_input);
	if (read_word(msh, lexer, &(new_redir->string)) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	if (new_redir->type == FD_HEREDOC && lex_heredoc(msh, new_redir) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	new_token->redir = new_redir;
	lexer->last_tk_type = TK_REDIR;
	return (SUCCESS);
}
