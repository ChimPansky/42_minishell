#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"

#define EXIT_COMMAND_NOT_FOUND		127
#define  EXIT_PERMISSION_DENIED		126

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

typedef struct s_var
{
	char *name;
	char *value;
}	t_var;

typedef t_list t_variables;

typedef struct s_msh
{
	char			*rl_input;
	char			pwd[PATH_MAX + 1];
	char			*prompt;
	const char		*mult_line_prompt;
	bool			mult_line_input;
	int				in_fd;
	int				out_fd;
	int				err_fd;
	int				last_exit_code;
	int				pid_to_wait;
	int				err_number;
	char			*err_info;
	t_tokens		*tokens;
	t_token			*last_token;
	t_command_chain	*commands;
	t_variables 	*env;
	t_variables		*locals;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

//minishell.c
// for libft:
char **strings_append(char **strings, char *appendix);
void str_print(char **strings);
char    *add_to_word(char **word, char new_char);

// TODO Vova:
int execute(t_msh *msh, t_command_chain *cmds);

// init.c
void	init(t_msh *msh, char **envp);

// update.c
void	update_pwd(t_msh *msh);
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

// pipex.c
void 	execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments);

// built_ins/built_in.c
t_built_in get_built_in_by_name(char *func_name);

// lexer.c
int 	lexer(t_msh *msh, char *input);

// heredoc.c
int 	read_heredocs(t_msh *msh);
int		process_here_doc(char **document, char *limiter);

// expander.c
int 	expander(t_msh *msh);

// parser.c
int 	parser(t_msh *msh);

// destroy.c
void	destroy(t_msh *msh);

// list_tokens.c
t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
						char *str, t_redir_detail *redir);
void 		destroy_token(void *token_void);
void	print_tokens(t_tokens **tokens);

// list_commands.c
t_simple_command	*command_add(t_command_chain **commands, char **cmd_with_args, t_list *redirections);
void 	destroy_command(void *command_void);
void	print_commands(t_command_chain **commands);

// list_variables.c
t_var 		*var_find(t_variables *vars, const char *name);
char		*var_get_value(t_variables *vars, const char *name);
t_var		*var_set(t_variables **vars_p, const char *name, const char *value);
t_var 		*var_add(t_variables **vars_p, const char *name, const char *value);
void 		var_delete(t_variables **vars_p, const char *name);
t_variables *vars_init_from_envp(char **envp);
char 		**vars_convert_to_array(t_variables *vars);

// strings.c:
bool    is_shell_space(char c);
bool    is_token_seperator(char c);
bool    is_var_separator(char c);
char    *remove_quotes(char **str);

#endif
