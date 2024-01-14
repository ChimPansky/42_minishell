#include "minishell.h"
#include "libft.h"

t_redir_detail	*redir_detail_create(t_redir_type fd_type, char *doc)
{
	t_redir_detail	*redir;

	redir = malloc(sizeof(t_redir_detail));
	if (!redir)
		return (NULL);
	ft_bzero(redir, sizeof(redir));
	redir->type = fd_type;
	if (doc)
	{
		redir->doc = ft_strdup(doc);
		if (!redir->doc)
			return (free(redir), NULL);
	}
	return (redir);
}

t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						char *str, t_redir_detail *redir)
{
	t_token 		*token;

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
		if (str)
		{
			token->str = ft_strdup(str);
			if (!token->str)
				return (free(token), NULL);
		}
		else
			token->str = NULL;
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
		if (token->redir->doc)
			free(token->redir->doc);
		free(token->redir);
	}
	else if (token->str)
		free(token->str);
	free(token);
}

void	print_tokens(t_tokens **tokens)
{
	t_tokens	*cur_list;
	t_token		*cur_token;
	char		*type_text;
	char		*fd_type;
	char		*fd_doc;
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
			fd_doc = NULL;
		}
		else if (cur_token->tk_type == TK_REDIR)
		{
			type_text = "REDIR";
			fd_doc = cur_token->redir->doc;
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
			fd_doc = NULL;
		}

		printf(" {T%d: Type: %s; Str: %s; FD_Type: %s; FD_Doc: %s)} -->\n",  i, type_text, cur_token->str, fd_type, fd_doc);
		cur_list = cur_list->next;
		i++;
	}
	printf("\n");
}

// void	print_tokens(t_tokens **tokens)
// {
// 	t_tokens	*cur_list;
// 	t_token		*cur_token;
// 	int			i;

// 	if (!tokens)
// 		return ;
// 	printf("token_list: ");
// 	cur_list = *tokens;
// 	i = 0;
// 	while (cur_list && cur_list->content)
// 	{
// 		cur_token = cur_list->content;
// 		printf(" {Token %d (%p): Type: %d; Str: %s Next: %p)} --> ",  i, cur_list, cur_token->tk_type, cur_token->str, cur_list->next);
// 		cur_list = cur_list->next;
// 		i++;
// 	}
// 	printf("\n");
// }
