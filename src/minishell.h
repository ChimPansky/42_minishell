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
	t_string		string;
	t_redir_type	type;
	bool			whitespace_expansion;
}		t_redir_detail;

typedef t_list t_redirections;

typedef enum e_token_type
{
	TK_NULL,
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
	t_token_type		tk_type;
	union {
		t_string		string;
		t_redir_detail	redir;
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
	int				last_token_type;
	t_tokens		*tokens;
	t_command_chain	*commands;
	t_variables 	*env;
	bool		 	done;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args, int fd_out);

//minishell.c
char    *add_to_word(char **word, char new_char); // try to get rid of this (instead use t_string with strings_add...)
char **strings_append(char **strings, char *appendix);
void strings_print(char **strings);

// TODO Vova:
int 	execute(t_msh *msh, t_command_chain *cmds);

// init.c
void	init(t_msh *msh, char **envp);

// update.c
void	update_prompt(t_msh *msh);
void	update(t_msh *msh);

//exit_error.c
void	ms_error_msg(int error_nr, char *err_info);
void	ms_exit(t_msh *msh, int error_nr);
void	ms_error(int error_nr);

// scratches.c
char 	*find_env(t_msh *msh, const char *var_name);
bool 	is_empty(const char *str);
void	print_splitted(char **splitted);

// built_ins/built_in.c
t_built_in get_built_in_by_name(char *func_name);

// lexer.c
int 	lex(t_msh *msh, char *input);

// heredoc.c
int 	read_heredocs(t_msh *msh, char **rl_chunk);
int		process_here_doc(t_msh *msh, char **document, char *limiter);

// expander/expander.c
int 	expand(t_msh *msh);

// parser.c
int 	parse(t_msh *msh);

// destroy.c
void	destroy(t_msh *msh);

// list_tokens.c
t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						t_string *str, t_redir_detail *redir);
void 		token_destroy(void *token_void);
void		print_tokens(t_tokens **tokens);

// list_commands.c
t_simple_command	*command_add(t_command_chain **commands, char **cmd_with_args, t_list *redirections);
void 				destroy_command(void *command_void);
void				print_commands(t_command_chain **commands);

// signals.c
void register_signals(void);

// strings.c
bool    is_shell_space(char c);
bool    is_token_seperator(char c);
bool    is_var_separator(char c);


// executor/executor.c
int 		execute(t_msh *msh, t_command_chain *cmds);

#endif
