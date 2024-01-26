#include "minishell.h"
#include "libft.h"


void	redir_detail_destroy(t_redir_detail *redir)
{
	if (!redir)
		return ;
	if (redir->string)
		string_destroy(redir->string);
	free(redir->word);
}

t_redir_detail	*redir_detail_create(t_redir_type fd_type, char *word)
{
	t_redir_detail	*redir;

	redir = malloc(sizeof(t_redir_detail));
	if (!redir)
		return (NULL);
	ft_bzero(redir, sizeof(t_redir_detail));
	redir->type = fd_type;
	if (word)
	{
		redir->word = ft_strdup(word);
		if (!redir->word)
			return (redir_detail_destroy(redir), NULL);
	}
	return (redir);
}
void	token_destroy(void *token_void)
{
	t_token *token = token_void;

	if (token->tk_type == TK_REDIR && token->redir)
		redir_detail_destroy(token->redir);
	if (token->string)
		string_destroy(token->string);
	free(token->word);
	free(token);
}

t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						char *word, t_redir_detail *redir)
{
	t_token 	*token;

	token = malloc(sizeof(t_token));	//ask vova why its not sizeof(t_token *)...
	if (!token)
		return NULL;
	ft_bzero(token, sizeof(token));
	token->tk_type = tk_type;
	if (tk_type == TK_REDIR)
	{
		token->redir = redir;
		if (!token->redir)
			return (token_destroy(token), NULL);
	}
	else
	{
		if (word)
		{
			token->word = ft_strdup(word);
			if (!token->word)
				return (token_destroy(token), NULL);
		}
	}
	t_tokens *new_token = ft_lstnew(token);
	if (!new_token)
		return (token_destroy(token), NULL);
	ft_lstadd_back(tokens, new_token);
	return (token);
}

void	print_tokens(t_tokens **tokens)
{
	t_tokens	*cur_list;
	t_token		*cur_token;
	char		*type_text;
	char		*tk_str;
	char		*fd_type;
	char		*fd_word;
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
		if (cur_token->string)
			tk_str = cur_token->string->buf;
		else
			tk_str = NULL;
		if (cur_token->tk_type == TK_WORD)
		{
			type_text = "WORD";
			fd_type = NULL;
			fd_word = NULL;
			fd_str = NULL;
		}
		else if (cur_token->tk_type == TK_REDIR)
		{
			type_text = "REDIR";
			fd_word = cur_token->redir->word;
			if (cur_token->redir->string)
				fd_str = cur_token->redir->string->buf;
			else
				fd_str = NULL;
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

		printf(" {T%d: Type: %s; word: %s; t_string: %s; FD_Type: %s; FD_word: %s FD_t_string: %s)} -->\n",  i, type_text, cur_token->word, tk_str, fd_type, fd_word, fd_str);
		cur_list = cur_list->next;
		i++;
	}
	printf("\n");
}
