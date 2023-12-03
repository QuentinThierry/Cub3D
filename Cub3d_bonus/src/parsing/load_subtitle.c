/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_subtitle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 19:26:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/03 14:38:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

#define READ_SIZE 100

static bool	_check_format(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (print_error("Wrong name of subtitle\n", 1), false);
	if (ft_strncmp(filename + (len - 4), ".txt", 4) != 0)
		return (print_error("Wrong format of subtitle\n", 1), false);
	return (true);
}

static char	*_read_subtitle(int fd, int *ret, int *empty)
{
	char	*tmp;
	char	*res;
	char	buffer[READ_SIZE + 1];

	res = NULL;
	buffer[READ_SIZE] = 0;
	*ret = read(fd, buffer, READ_SIZE);
	*empty = true;
	while (*ret > 0)
	{
		*empty = false;
		if (*ret != READ_SIZE)
			buffer[*ret] = 0;
		tmp = ft_strjoin(res, buffer);
		free(res);
		if (tmp == NULL)
			return (print_error("Malloc failed\n", 1), close(fd), NULL);
		res = tmp;
		*ret = read(fd, buffer, READ_SIZE);
	}
	return (res);
}

bool	load_subtitle(t_music_name *music)
{
	int		fd;
	int		ret;
	char	*res;
	int		empty;

	if (!_check_format(music->subtitle))
		return (false);
	fd = open(music->subtitle, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	res = _read_subtitle(fd, &ret, &empty);
	if (res == NULL && empty == false)
		return (false);
	free(music->subtitle);
	music->subtitle = res;
	close(fd);
	if (ret == -1)
		return (print_error(NULL, 0), false);
	return (true);
}
