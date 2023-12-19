/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:07:09 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 18:09:18 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_line
{
	char	buffer[BUFFER_SIZE];
	char	*next;
	ssize_t	ret;
	ssize_t	len_res;
	ssize_t	len_buf;
}	t_line;

char	*get_next_line(int fd);
ssize_t	find_end_line(char const *str, ssize_t len);
char	*fill_result(t_line *line, int fd, char *res, ssize_t len);
char	*ft_strjoin(t_line *line, char *begin, char *res);
char	*exception(t_line *line, char *res, ssize_t len);

#endif