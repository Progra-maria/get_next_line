/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricarr <maricarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 19:57:06 by maricarr          #+#    #+#             */
/*   Updated: 2023/02/13 13:28:26 by maricarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

char				*get_next_line(int fd);
void				read_and_stash(int fd, t_list **stash);
void				add_to_stash(t_list **stash, char *buffer, int char_read);
void				extract_line(t_list *stash, char **line);
void				generate_line(char **line, t_list *stash);
void				clean_stash(t_list **stash, t_list *last);
void				free_stash(t_list *stash);
t_list				*ft_lstlast(t_list *lst);
int					found_nl_char(t_list *stash);
unsigned long int	ft_strlen_ch(const char *str, char c);

#endif
