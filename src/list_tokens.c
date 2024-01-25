#include "minishell.h"
#include "libft.h"

t_redir_detail	*redir_detail_create(t_redir_type fd_type, char *word)
{
	t_redir_detail	*redir;
	t_string		string;

	redir = malloc(sizeof(t_redir_detail));
	if (!redir)
		return (NULL);
	ft_bzero(redir, sizeof(redir));
	redir->type = fd_type;
	if (word)
	{
		if (string_init(&string, word) != SUCCESS);
			return (free(redir), NULL);
		redir->string = &string;
	}
	else
		redir->string = NULL;
	return (redir);
}

t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						char *word, t_redir_detail *redir)
{
	t_token 	*token;
	t_string	string;

	token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	ft_bzero(token, sizeof(token));
	token->tk_type = tk_type;
	if (tk_type == TK_REDIR)
	{
		token->redir = redir;
		if (!token->redir)
			return (free(token), NULL);
	}
	else
	{
		if (word)
		{
			if (string_init(&string, word) != SUCCESS);
				return (free(token), NULL);
			token->string = &string;
		}
		else
			token->string = NULL;
	}
	t_tokens *new_token = ft_lstnew(token);
	if (!new_token)
		return (destroy_token(token), NULL);
	ft_lstadd_back(tokens, new_token);
	return (token);
}

void	destroy_token(void *token_void)
{
	t_token *token = token_void;

	if (token->tk_type == TK_REDIR && token->redir)
	{
		if (token->redir->string)
			string_destroy(token->redir->string);
		free(token->redir);
	}
	else if (token->string)
		string_destroy(token->string);
	free(token);
}

void	print_tokens(t_tokens **tokens)
{
	t_tokens	*cur_list;
	t_token		*cur_token;
	char		*type_text;
	char		*fd_type;
	char		*fd_str;
	int			i;


	if (!tokens)
		return ;
	printf("token_list:\n");
	cur_list = *tokens;
	i = 1;
	while (cur_list && cur_list->content)
	{
		cur_token = cur_list->content;

		if (cur_token->tk_type == TK_WORD)
		{
			type_text = "WORD";
			fd_type = NULL;
			fd_str = NULL;
		}
		else if (cur_token->tk_type == TK_REDIR)
		{
			type_text = "REDIR";
			fd_str = cur_token->redir->string;
			if (cur_token->redir->type == FD_IN)
				fd_type = "<";
			else if (cur_token->redir->type == FD_HEREDOC)
				fd_type = "<<";
			else if (cur_token->redir->type == FD_OUT_TRUNC)
				fd_type = ">";
			else if (cur_token->redir->type == FD_OUT_APPEND)
				fd_type = ">>";
		}
		else if (cur_token->tk_type == TK_PIPE)
		{
			type_text = "PIPE";
			fd_type = NULL;
			fd_str = NULL;
		}

		printf(" {T%d: Type: %s; Str: %s; FD_Type: %s; FD_Str: %s)} -->\n",  i, type_text, cur_token->string->buf, fd_type, fd_str);
		cur_list = cur_list->next;
		i++;
	}
	printf("\n");
}
