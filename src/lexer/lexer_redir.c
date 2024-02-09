/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:15:28 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/09 13:00:08 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"
#include <signal.h>

// cmd d and cmd c check
// if success return SUCCESS
// + expansionon the expansion step
// + empty delimeter case


// newlines appear as ^J in rl_history.
// this is a limitation of the rl_history library and can not be changed...
// so for now we wont use this function...
// static int	add_heredoc_to_history(t_string *rl_mainloop_input,
// 	t_charptr_array heredoc_content)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < heredoc_content.sz)
// 	{
// 		if (string_add_chr(rl_mainloop_input, '\n') != SUCCESS
// 			|| string_add_str(rl_mainloop_input, heredoc_content.buf[i]) != SUCCESS)
// 			return (!SUCCESS);
// 		i++;
// 	}
// 	return (SUCCESS);
// }
static int	lex_heredoc(t_msh *msh, t_redir_detail *redir)
{
    char    	*line;
	t_string	line_w_nl;

	if (!(ft_strlen(redir->string.buf) > string_remove_quotes(&redir->string)))
		redir->expand_heredoc = true;
	while (1)
	{
		line = readline_wrapper(PROMPT_HEREDOC);
		ft_putstr_fd("heredoc line has been read\n", STDOUT_FILENO);
		if (check_for_signals(msh))
		{
			ft_putstr_fd("Lex heredoc ctr + C", STDOUT_FILENO);
			return (free(line), !SUCCESS);
		}
		if (line == NULL)
        {
            if (errno)
            	return (perror("readline heredoc"), !SUCCESS);
			ft_printf("\nwarning: here-document delimited "
				"by end-of-file (wanted '%s')\n", redir->string.buf);
        }
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
	else 	if (ft_strncmp(">>", lexer->pos_in_input, 2) == SUCCESS)
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
	else {
		lexer->redir_type = FD_NULL;
	}
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
	new_token->redir = new_redir;
	lexer->last_tk_type = TK_REDIR;
	if (new_redir->type == FD_HEREDOC && lex_heredoc(msh, new_redir) != SUCCESS)
		return (redir_destroy(new_redir), !SUCCESS);
	return (SUCCESS);
}
