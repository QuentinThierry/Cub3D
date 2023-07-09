/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:06:58 by jvigny            #+#    #+#             */
/*   Updated: 2022/11/24 11:19:55 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	find_end_line(char const *str, ssize_t len)
{
	ssize_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*fill_result(t_line *line, int fd, char *res, ssize_t len)
{
	while (len == -1)
	{
		line->ret = read(fd, line->buffer, BUFFER_SIZE);
		if (line->ret == -1)
			return (NULL);
		if (line->ret == 0)
			return (res);
		len = find_end_line(line->buffer, line->ret);
		if (len == -1)
		{
			line->next = NULL;
			line->len_buf = line->ret;
			res = ft_strjoin(line, line->buffer, res);
		}
		else
		{
			line->next = line->buffer + len + 1;
			if (len + 1 >= line->ret)
				line->next = NULL;
			line->len_buf = len + 1;
			res = ft_strjoin(line, line->buffer, res);
		}
	}
	return (res);
}

char	*ft_strjoin(t_line *line, char *begin, char *res)
{
	char	*res_f;
	ssize_t	len;
	ssize_t	j;

	len = line->len_res + line->len_buf;
	j = 0;
	res_f = malloc(sizeof(char) * len + 1);
	if (res_f == 0)
		return (0);
	while (res != 0 && j < line->len_res)
	{
		res_f[j] = res[j];
		j++;
	}
	line->len_res = len;
	len = 0;
	while (begin != 0 && j < line->len_res)
	{
		res_f[j] = begin[len];
		j++;
		len++;
	}
	res_f[j] = 0;
	free(res);
	return (res_f);
}

char	*exception(t_line *line, char *res, ssize_t len)
{
	line->len_buf = len + 1;
	res = ft_strjoin(line, line->next, res);
	line->next = line->next + len + 1;
	if (line->next >= line->buffer + line->ret)
		line->next = NULL;
	return (res);
}
