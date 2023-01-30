
#include "get_next_line.h"

int found_nl_char(t_list *stash)
{
    int i;
    t_list *current;

    
    if (stash == NULL)
            return (0);
    current = ft_lstlast(stash);
    i = 0;
    while (current->content[i])
    {
        if (current->content[i] == '\n')
            return (1);
        i++;
    }
    return (0);
}

void    generate_line(char **line, t_list *stash)
{
    int i;
    int len;

    len = 0;
    while (stash)
    {//if (stash->content[0] == '\0')
        i = 0;
        while (stash->content[i])
        {
            if (stash->content[i] == '\n')
            {
                len++;
                break;
            }
            len++;
            i++;
        }
        stash = stash->next;
    }
    *line = malloc(sizeof(char) * (len + 1));
}

t_list	*ft_lstlast(t_list *lst)
{   
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void    free_stash(t_list *stash)
{
    t_list  *current;
    t_list  *nxt;

    current = stash;
    while (current)
    {
        free(current->content);
        nxt = current->next;
        free(current);
        current = nxt;
    }
}

unsigned long int	ft_strlen(const char *str)
{
	unsigned long int	i;

	i = 0;
	while (str[i] != '\0')
	{
	i++;
	}
	return (i);
}
