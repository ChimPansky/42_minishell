#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <signal.h>
# include "ms_macros.h"
# include "libft.h"
# include "ft_string.h"
# include "structures/list_variables.h"
# include "structures/list_tokens.h"
# include "structures/list_commands.h"

# define PROMPT_MAX_LEN  99 //255
# define PROMPT_INVITATION "$ "
# define PROMPT_HEREDOC "> "

# define EXIT_COMMAND_NOT_FOUND		127
# define EXIT_PERMISSION_DENIED		126
# define EXIT_SIG_INT				130
#   define	ER_UNEXPECTED_TOKEN     2

extern sig_atomic_t	g_signal_no;

typedef enum	e_signal_mode
{
	SIG_READLINE_MAIN,
	SIG_READLINE_HEREDOC,
	SIG_NON_INTERACTIVE,
	SIG_EXECUTOR
}				t_signal_mode;

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
void		ms_destroy(t_msh *msh);
void		ms_stop(t_msh *msh);

// prompt.c
void		update_prompt(t_msh *msh);

//exit_error.c
void		error_unexp_tk_s(t_msh *msh, char *token);
void		error_unexp_tk_c(t_msh *msh, char symbol);

// built_ins/built_in.c
t_built_in	get_built_in_by_name(char *func_name);

// lexer/lexer.c
int 		lex(t_msh *msh, t_tokenlist **tokens_p, char *input);

// parser.c
int 	parse_and_execute(t_msh *msh, t_tokenlist *tokens);

// helpers
char	*readline_wrapper(char *prompt);

// signals.c
//int 	register_signals(void);
void	configure_signals(t_signal_mode sig_mode);
int		check_for_signals(t_msh *msh);

// helpers/string_utils.c
bool    is_token_seperator(char c);
bool	is_special_var_name(char c);
bool	is_var_name_start(char c);
bool    is_var_separator(char c);
size_t	str_remove_quotes(char **str);
size_t	string_remove_quotes(t_string *string);

// executor/executor.c
int 		execute(t_msh *msh, t_cmdlist *cmds);

#endif
