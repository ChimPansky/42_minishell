#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include "ms_macros.h"
# include "libft.h"
# include "ft_string.h"
# include "structures/list_variables.h"
# include "structures/list_tokens.h"
# include "structures/list_commands.h"

# define PROMPT_MAX_LEN  99 //255
# define PROMPT_INVITATION "$ "

# define EXIT_COMMAND_NOT_FOUND		127
# define EXIT_PERMISSION_DENIED		126
# define EXIT_SIG_INT				130

extern bool g_sigint_received;

typedef enum e_ms_error
{
    ER_UNDEFINED,
    ER_QUOTES,
	ER_UNEXPECTED_TOKEN,
	ER_READLINE,
	ER_MALLOC,
	ER_AMBIGUOUS_REDIRECT
}		t_ms_error;

typedef struct s_msh
{
	t_string		prompt;
	int				last_exit_code;
	t_varlist 		*env;
	bool		 	done;
}			t_msh;

typedef int (*t_built_in)(t_msh *msh, char **cmd_with_args, int fd_out);

// app.c
void		ms_init(t_msh *msh, char **envp);
void		ms_destroy_and_exit(t_msh *msh);
void		ms_stop(t_msh *msh);

// prompt.c
void		update_prompt(t_msh *msh);

//exit_error.c
void		error_unexpected_token(char symbol);
void		ms_error_msg(int error_nr, char *err_info);
void		ms_error(int error_nr);

// built_ins/built_in.c
t_built_in	get_built_in_by_name(char *func_name);

// lexer/lexer.c
int 		lex(t_msh *msh, t_tokenlist **tokens_p, char *input);

// parser.c
int 	parse_and_execute(t_msh *msh, t_tokenlist *tokens);

// helpers
char	*readline_wrapper(char *prompt);

// signals.c
void 		register_signals(void);

// helpers/string_utils.c
bool    is_token_seperator(char c);
bool	is_special_var_name(char c);
bool	is_var_name_start(char c);
bool    is_var_separator(char c);
void	str_remove_quotes(char **str);

// executor/executor.c
int 		execute(t_msh *msh, t_cmdlist *cmds);

#endif
