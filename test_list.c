#include "libft/include/libft.h"
#include <stdio.h>

void	print_list(t_list **list)
{
	t_list	*cur_list;
	int		*val;
	int			i;

	if (!list)
	{
		printf("(NULL list)\n");
		return ;
	}
	cur_list = *list;
	if (!cur_list)
	{
		printf("(empty list)\n");
		return ;
	}
	i = 0;
	while (cur_list && cur_list->content)
	{
		val = cur_list->content;
		printf("%p <-- {Node %d (%p): Val: %d} -->%p\n", cur_list->prev, i, cur_list, *val, cur_list->next);
		cur_list = cur_list->next;
		i++;
	}
}
void list_del(void *list_node)
{
	(void) list_node;
	return ;
}

char **strings_append(char **strings, char *appendix)
{
	char	**new_strings;
	size_t	s_count;

	s_count = 0;
	if (!appendix)
		return (strings);
	while (strings && strings[s_count])
		s_count++;
	new_strings = ft_calloc(s_count + 2, sizeof(char *));
	if (!new_strings)
		return (strings);
	s_count = 0;
	while (strings && strings[s_count])
	{
		new_strings[s_count] = strings[s_count];
		s_count++;
	}
	new_strings[s_count] = ft_strdup(appendix);
	new_strings[s_count + 1] = NULL;
	return (new_strings);
}

void str_print(char **strings)
{
	int i;
	i = 0;

	if (!strings)
		printf("{NULL}\n");
	while (strings && strings[i])
	{
		printf("%d: %s\n", i, strings[i]);
		i++;
	}
	printf("%d: %s\n", i, "NULL");
}
int	main()
{
	t_list	*my_list;
	t_list	*new_node;
	char **strings;

	strings = NULL;
	str_print(strings);
	printf("before append\n");
	strings = strings_append(strings, "hello");
	strings = strings_append(strings, "-asd");
	strings = strings_append(strings, "thats it");
	printf("after append\n");
	str_print(strings);
	int	a;
	int b;
	int c;

	a = 42;
	b = 24;
	c = 242;

	// new_node = ft_lstnew(&a);
	// my_list = new_node;


	// ft_lstadd_back(&my_list, new_node = ft_lstnew(&b));
	// ft_lstadd_front(&my_list, ft_lstnew(&c));
	// //printf("%d", *(int*)(my_list->next->content));
	// print_list(&my_list);
	// printf("\n%d\n", *(int*)(new_node->content));
	// ft_lstdel_node(&my_list, new_node, list_del);
	// print_list(&my_list);
	//ft_lstclear(&my_list, list_del);
	// print_list(&my_list);

}
