
# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdio.h>
#include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

char    *get_next_line(int fd);
void    read_and_stash(int fd, t_list **stash);
void    add_to_stash(t_list **stash, char *buffer, int n_char_char_read);
void    extract_line(t_list *stash, char **line);
void    extract_line();
void    generate_line(char **line, t_list *stash);
void    clean_stash();
void    free_stash(t_list *stash);
t_list  *ft_lstlast(t_list *lst);
unsigned long int	ft_strlen(const char *str);

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

void	read_and_stash(int fd, t_list **stash)
{
	char	*buff;
	int		char_read;

    char_read = 0;
	while (!found_nl_char(*stash) && char_read >= 0)
	{   
        buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
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
			return ;
		}
		add_to_stash(stash, buff, char_read);
		free(buff);
	}
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
        if (new_node->content == NULL)
            return;
        i = 0;
        while ((buffer[i] && i < n_char_char_read) && buffer[i] != EOF)
        {
            new_node->content[i] = buffer[i];
            i++;
        }
        if (buffer[i] == EOF)
            new_node->content[i] = EOF;
        else
            new_node->content[i] = '\0';
        if (*stash == NULL)
            {
                *stash = new_node;
                return ;
            }
        last = ft_lstlast(*stash);
        last->next = new_node;
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
    free_stash(*stash);
    *stash = clean_node;
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

t_list	*ft_lstlast(t_list *lst)
{   
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*this function function calculates the length of a given string*/
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


char    *get_next_line(int fd)
{
    static t_list   *stash;
    char            *line;

    line = NULL;
    if (stash  == NULL || !found_nl_char(stash))
    {
        if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
            return (NULL);
        read_and_stash(fd, &stash);
    }
    if (line == NULL && ((stash == NULL) || (stash->content[0] == '\0' && stash->next == NULL)))
        return (NULL);
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

int main(void)
{
    int fd;
    char *line;

    fd = open("FILES/1char.txt", O_RDONLY);
    while(fd >= 0)
    {
        line = get_next_line(fd);
        if (line == NULL)
        {
            printf("NULL");
            break;
        }
        printf("%s", line);
        free(line);
    }
    system("leaks a.out");
    return (0);
}