/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:07:06 by jvigny            #+#    #+#             */
/*   Updated: 2022/11/24 10:59:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_line	line;
	ssize_t			len;
	char			*res;

	len = -1;
	res = NULL;
	if (line.next == NULL || line.next == line.buffer)
		res = fill_result(&line, fd, res, len);
	else
	{
		line.len_buf = line.ret - (line.next - line.buffer);
		len = find_end_line(line.next, line.len_buf);
		if (len == -1)
		{
			res = ft_strjoin(&line, line.next, res);
			line.next = NULL;
			res = fill_result(&line, fd, res, len);
		}
		else
			res = exception(&line, res, len);
	}
	line.len_res = 0;
	line.len_buf = 0;
	return (res);
}
