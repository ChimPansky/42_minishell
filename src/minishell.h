#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"
# include "ft_string.h"
# include "structures/list_variables.h"

# define PROMPT_MAX_LEN  99 //255
# define PROMPT_INVITATION "$ "

# define EXIT_COMMAND_NOT_FOUND		127
# define EXIT_PERMISSION_DENIED		126
# define EXIT_SIG_INT				130

extern bool g_sigint_received;



// PARSER STUFF START
typedef enum e_redir_type
{
    FD_IN,
    FD_HEREDOC,
    FD_OUT_TRUNC,
    FD_OUT_APPEND
}		t_redir_type;

// str will be input read with heredoc instead of heredoc delimiter
typedef struct s_redir_detail
{
	t_redir_type	type;
	char			*str;
}		t_redir_detail;

typedef t_list t_redirections;

typedef enum e_token_type
{
    TK_WORD,
    TK_REDIR,
	TK_PIPE,
	TK_BRACKET,
	TK_LOGIC_AND,
    TK_LOGIC_OR
}		t_token_type;

typedef enum e_ms_error
{
    ER_UNDEFINED,
    ER_QUOTES,
	ER_UNEXPECTED_TOKEN,
	ER_READLINE,
	ER_MALLOC,
	ER_AMBIGUOUS_REDIRECT
}		t_ms_error;

typedef t_list t_tokens;

typedef struct s_token
{
	t_token_type	tk_type;
	union {
		char			*word;
		t_redir_detail	*redir;
		t_tokens		*subshell;		//	only for bonus
		int				sub_exit_code;	//	only for bonus
	};
}	t_token;

typedef t_list t_command_chain;

typedef struct s_simple_command
{
	char	**cmd_with_args;
	t_list	*redirections; // list of t_redir_details...
}		t_simple_command;

typedef struct s_msh
{
	char			*rl_input;
	t_string		prompt;
	const char		*mult_line_prompt;
	bool			mult_line_input;
	int				last_exit_code;
	int				err_number;
	char			*err_info;
	t_tokens		*tokens;
	t_token			*last_token;
	t_command_chain	*commands;
	t_variables 	*env;
	bool		 	done;
}		t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args, int fd_out);

//minishell.c
// for libft:
char **strings_append(char **strings, char *appendix);
void str_print(char **strings);

// app.c
void	ms_init(t_msh *msh, char **envp);
void	ms_destroy_and_exit(t_msh *msh);
void	ms_stop(t_msh *msh);

// prompt.c
void	update_prompt(t_msh *msh);

//exit_error.c
void	ms_error_msg(int error_nr, char *err_info);
void	ms_error(int error_nr);

// scratches.c
char 	*find_env(t_msh *msh, const char *var_name);
bool 	is_empty(const char *str);
void	print_splitted(char **splitted);

// built_ins/built_in.c
t_built_in get_built_in_by_name(char *func_name);

// lexer.c
int 	lexer(t_msh *msh, char *input);
char    *add_to_word(char **word, char new_char);

// expander.c
int 	expander(t_msh *msh);

// parser.c
int 	parser(t_msh *msh);

// list_tokens.c
t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						char *str, t_redir_detail *redir);
void 		destroy_token(void *token_void);
void	print_tokens(t_tokens **tokens);

// list_commands.c
t_simple_command	*command_add(t_command_chain **commands, char **cmd_with_args, t_list *redirections);
void 	destroy_command(void *command_void);
void	print_commands(t_command_chain **commands);

// signals.c
void register_signals(void);

// executor/executor.c
int 		execute(t_msh *msh, t_command_chain *cmds);

#endif
