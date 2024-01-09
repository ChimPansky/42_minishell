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

typedef structs_redir_descr
{
	t_redir_type	type;
	char			*doc;
}		t_redir_descr;

// words: sindle quote, dquote, wildcard
typedef enum e_token_type
{
    TK_WORD,
    TK_REDIR,
	TK_PIPE,
	TK_BRACKET,
	TK_LOGIC_AND,
    TK_LOGIC_OR
}		t_token_type;

typedef t_list t_tokens;

// maybe add union
typedef struct s_token
{
	t_token_type	tk_type;
	union {
		char			*str;
		t_redir_descr	redir;
		t_tokens		*subshell;
		int				sub_exit_code;
	};
}	t_token;


typedef t_list t_command_chain;

typedef struct s_simple_command
{
	char	**cmd_with_args;
	t_list	redirections;
}		t_simple_command;

int execute(t_msh *msh, t_long_command *cmd);

// struct for isolated command including arguments
// e.g.: "ls -l"
// typedef struct s_command
// {
// 	char		*cmd_with_args;
// 	// redirections
// 	s_command	*next;
// }			t_command;

// struct for several commands (can we use t_list instead?)
// e.g.: "ls -l | cat -e"
// e.g.: "cat | grep test < infile.txt"
// ...
// typedef struct s_command_chain
// {
// 	t_command	*cmd_list;


// }			t_command_chain;
// PARSER STUFF END

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
	int				in_fd;
	int				out_fd;
	int				err_fd;
	int				last_exit_code;
	int				pid_to_wait;
	t_tokens		*tokens;
	t_variables 	*env;
	t_variables		*locals;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args);

// init.c
void	init(t_msh *msh, char **envp);

// update.c
void	update_pwd(t_msh *msh);
void	update_prompt(t_msh *msh);
void	update(t_msh *msh);

//exit_error.c
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

// parser.c
int 	parse(t_msh *msh, char *input);
int 	new_parse(t_msh *msh, t_tokens *tokens, char *input);
void	print_tokens(t_tokens **tokens);

// destroy.c
void	destroy(t_msh *msh);

// tokens.c
t_token		*token_add(t_tokens **tokens, t_token_type tk_type,
			t_redir_type fd_type, char *str);
void 		destroy_token(void *token_void);

// variables.c
t_var 		*var_find(t_variables *vars, const char *name);
char		*var_get_value(t_variables *vars, const char *name);
t_var		*var_set(t_variables **vars_p, const char *name, const char *value);
t_var 		*var_add(t_variables **vars_p, const char *name, const char *value);
void 		var_delete(t_variables **vars_p, const char *name);
t_variables *vars_init_from_envp(char **envp);
char 		**vars_convert_to_array(t_variables *vars);

#endif
