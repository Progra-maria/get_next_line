
#include "get_next_line.h"

char    *get_next_line(int fd)
{
    static t_list   *stash;
    char            *line;

    //printf("STASH: %p\n", stash);
    line = NULL;
    //printf("LINE: %p\n", line);
    if (stash  == NULL || !found_nl_char(stash))
    {
        if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
            return (NULL);
        read_and_stash(fd, &stash);
    }
    if (line == NULL && ((stash == NULL) || (stash->content[0] == '\0' && stash->next == NULL)))
        {   
            free(stash);
            stash = NULL;
            free(line);
            return (NULL);
        }
    extract_line(stash, &line);
    clean_stash(&stash);
    if (line == NULL)
    {
        free(stash);
        stash = NULL;
        free(line);
        return (NULL);
    }
    return (line);
}

void	read_and_stash(int fd, t_list **stash)
{
	char	*buff;
	int		char_read;

    char_read = 0;
	while (!found_nl_char(*stash) && char_read >= 0)
	{   
        buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        //printf("BUFFER: %p\n", buff);
        char_read = (int)read(fd, buff, BUFFER_SIZE);
        if (char_read <= 0)
        {
            free(buff);

            return ;
        }
        if (char_read <= BUFFER_SIZE && char_read > 0)
            buff[char_read] = '\0';
		if (*stash == NULL && char_read == 0)
		{
			free(buff);
            buff = NULL;
            //printf("BUFFER DESPUÉS DE FREE: %p\n", buff);
			return ;
		}
		add_to_stash(stash, buff, char_read);
		free(buff);
	}
}

void    extract_line(t_list *stash, char **line)
{
    int i;
    int j;

    if (stash == NULL)
        return;
    generate_line(line, stash);
    if (*line == NULL)
            return;
    j = 0;
    while (stash)
    {
        i = 0;
        while (stash->content[i])
        {
            if (stash->content[i] == '\n')
            {
                (*line)[j++] = stash->content[i];
                break;
            }
            (*line)[j++] = stash->content[i++]; 
        }
        stash = stash->next;
    }
    (*line)[j] = '\0';
}

void    clean_stash(t_list **stash)
{
    t_list  *last;
    t_list  *clean_node;
    int i;
    int j;

    clean_node = malloc(sizeof(t_list));
    if (*stash == NULL || clean_node == NULL)
        return;
    clean_node->next = NULL;
    last = ft_lstlast(*stash);
    i = 0;
    while(last->content[i] && last->content[i] != '\n')
        i++;
    if(last->content && last->content[i] == '\n')
        i++;
    clean_node->content = malloc(sizeof(char) * (ft_strlen(last->content) - i + 1));
    if (clean_node->content == NULL)
        return;
    j = 0;
    while (last->content[i])
        clean_node->content[j++] = last->content[i++];
    clean_node->content[j] = '\0';
    //printf("CLEAN_NODE: %p\n", clean_node);
    free_stash(*stash);
    *stash = clean_node;
}
void    add_to_stash(t_list **stash, char *buffer, int n_char_char_read)
{       
        t_list *last;
        t_list *new_node;
        int i;

        new_node = malloc(sizeof (t_list));
        if (new_node == NULL)
            return ;
        new_node->next = NULL;
        new_node->content = malloc(sizeof(char) * (n_char_char_read + 1));
        //printf("NEW_NODE: %p\n", new_node);
        if (new_node->content == NULL)
            return;
        i = 0;
        while ((buffer[i] && i < n_char_char_read) && buffer[i] != EOF)
        {
            new_node->content[i] = buffer[i];
            i++;
        }
        new_node->content[i] = '\0';
        if (*stash == NULL)
            {
                *stash = new_node;
                return ;
            }
        last = ft_lstlast(*stash);
        last->next = new_node;
}