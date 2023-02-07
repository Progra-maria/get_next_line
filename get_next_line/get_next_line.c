/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricarr <maricarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 19:24:25 by maricarr          #+#    #+#             */
/*   Updated: 2023/02/07 09:50:22 by maricarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * Read from the file descriptor until a newline character is found, then return
 * the line
 * 
 * @param fd file descriptor
 * 
 * @return A line of text from a file descriptor.
 */
char	*get_next_line(int fd)
{
	static t_list	*stash;
	char			*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
	{
		free_stash(stash);
		stash = NULL;
		return (NULL);
	}
	if (stash == NULL || !found_nl_char(stash))
		read_and_stash(fd, &stash);
	if (line == NULL && stash == NULL)
	{
		free_stash(stash);
		return (NULL);
	}
	extract_line(stash, &line);
	clean_stash(&stash);
	if (line == NULL)
		free_stash(stash);
	return (line);
}

/**
 * Reads from a file descriptor and stores the data in a linked list
 * 
 * @param fd the file descriptor to read from
 * @param stash a pointer to a pointer to a linked list.
 * 
 * @return a pointer to a string.
 */
void	read_and_stash(int fd, t_list **stash)
{
	char	*buff;
	int		char_read;

	char_read = 1;
	while (!found_nl_char(*stash) && char_read >= 1)
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
			buff = NULL;
			return ;
		}
		add_to_stash(stash, buff, char_read);
		free(buff);
	}
}

/**
 * It takes a linked list of strings, and a pointer to a string, and it
 * copies the contents of the linked list into the string, 
 * and then it returns
 * 
 * @param stash a linked list of strings, each string is a line from the file
 * @param line a pointer to a pointer to a char.
 * 
 * @return A line of text from a file.
 */
void	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	generate_line(line, stash);
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}


/**
 * It takes the last node of the stash, and if the last node 
 * has a newline character in it, it removes
 * the newline character and everything before it, and then
 * frees the rest of the stash
 * 
 * @param stash a pointer to a pointer to a t_list.
 * 
 * @return the number of bytes read.
 */
void	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	last = ft_lstlast(*stash);
	i = ft_strlen_ch(last->content, '\n');
	if ((ft_strlen_ch(last->content, '\0') - i) == 0)
	{
		free_stash(*stash);
		*stash = NULL;
		return ;
	}
	clean_node = malloc(sizeof(t_list));
	if (*stash == NULL || clean_node == NULL)
		return ;
	clean_node->next = NULL;
	clean_node->content = malloc(sizeof(char) * \
			((ft_strlen_ch(last->content, '\0')) + 1));
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}

/**
 * It takes a pointer to a pointer to a linked list, a buffer, 
 * and the number of characters read from the buffer, and adds
 * a new node to the linked list with the contents of the buffer
 * 
 * @param stash a pointer to a pointer to a t_list. This is the list
 * that will contain the data read
 * from the file descriptor.
 * @param buffer the buffer that was read from the file descriptor
 * @param char_read the number of characters read from the file descriptor
 * 
 * @return the last node of the list.
 */
void	add_to_stash(t_list **stash, char *buffer, int char_read)
{
	t_list	*last;
	t_list	*new_node;
	int		i;

	new_node = malloc(sizeof (t_list));
	if (new_node == NULL)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * (char_read + 1));
	if (new_node->content == NULL)
		return ;
	i = 0;
	while ((buffer[i] && i < char_read))
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