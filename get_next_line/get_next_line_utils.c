/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricarr <maricarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:21:32 by maricarr          #+#    #+#             */
/*   Updated: 2023/02/06 22:47:03 by maricarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * It returns the index of the first newline character in the last element of 
 * the stash list
 * 
 * @param stash a linked list of strings
 * 
 * @return The number of characters in the last node of the stash list, starting
 * from the first newline character.
 */
int	found_nl_char(t_list *stash)
{
	int		i;
	t_list	*current;

	if (stash == NULL)
		return (0);
	current = ft_lstlast(stash);
	i = 0;
	while (current->content[i])
	{
		if (current->content[i] == '\n')
		{
			i++;
			return (i);
		}
		i++;
	}
	return (0);
}

/**
 * It counts the number of characters in the stash list until it finds a newline
 * character
 *
 * @param line the address of a pointer to a character.
 * @param stash a linked list of strings
 */
void	generate_line(char **line, t_list *stash)
{
	int	i;
	int	len;

	len = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				len++;
				break ;
			}
			len++;
			i++;
		}
		stash = stash->next;
	}
	*line = malloc(sizeof(char) * (len + 1));
}

/**
 * Ft_lstlast returns the last element of a linked list.
 * 
 * @param lst The list to be searched.
 * 
 * @return The last element of the list.
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/**
 * It frees the stash
 * 
 * @param stash a pointer to the first element of the stash
 */
void	free_stash(t_list *stash)
{
	t_list	*current;
	t_list	*nxt;

	current = stash;
	while (current)
	{
		free(current->content);
		nxt = current->next;
		free(current);
		current = nxt;
	}
}

/**
 * It returns the length of a string up to a given character
 * 
 * @param str The string to be searched.
 * @param c the character to search for
 * 
 * @return The length of the string until the character is found.
 */
unsigned long int	ft_strlen_ch(const char *str, char c)
{
	unsigned long int	i;

	i = 0;
	if (c == '\n')
	{
		while (str[i] && str[i] != '\n')
			i++;
		if (str && str[i] == '\n')
			i++;
	}
	if (c == '\0')
	{
		while (str[i] != '\0')
			i++;
	}
	return (i);
}
