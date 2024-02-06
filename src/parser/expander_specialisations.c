#include "parser.h"

static int	expand_var_in_glob(
	const char *var_content,
	t_string *replace,
	t_charptr_array *arr)
{
	const char	*pos_in_var = var_content;

	while (*pos_in_var)
	{
		if (ft_isspace(*pos_in_var))
		{
			if (!string_is_empty(replace))
			{
				if (SUCCESS
					!= charptr_array_add_allocated_str(arr, &replace->buf))
					return (perror("expand_var_in_glob: "
							"charptr_array_add_allocated_str"), !SUCCESS);
				if (SUCCESS != string_init(replace, ""))
					return (perror("expand_var_in_glob: "
							"string_init"), !SUCCESS);
			}
		}
		else
		{
			if (SUCCESS != string_add_chr(replace, *pos_in_var))
				return (perror("expand_var_in_glob: string_add_chr"), !SUCCESS);
		}
		pos_in_var++;
	}
	return (SUCCESS);
}

int	expand_singleq(t_expander *expander)
{
	expander->pos++;
	while (*expander->pos && *expander->pos != '\'')
		if (SUCCESS != string_add_chr(&expander->replace, *expander->pos++))
			return (perror("expand_singleq: string_add_chr"), !SUCCESS);
	if (!*expander->pos)
		return (ft_printf_err("CRIT ERR: unexpected eol\n"), !SUCCESS);
	expander->pos++;
	return (SUCCESS);
}

int	expand_variable(t_msh *msh, t_expander *expander, t_charptr_array *arr)
{
	char	*var_content;

	expander->pos++;
	var_content = get_var_content(msh, &expander->pos);
	if (expander->glob)
		return (expand_var_in_glob(var_content, &expander->replace, arr));
	if (SUCCESS != string_add_str(&expander->replace, var_content))
		return (perror("expand_variable: string_add_chr"), !SUCCESS);
	return (SUCCESS);
}
