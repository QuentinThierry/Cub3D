/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 19:35:38 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 19:57:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/get_next_line.h"

static char	**_init_map(t_vector2 len)
{
	int		i;
	char	**res;

	i = 0;
	res = ft_calloc(len.y, sizeof(char *));
	if (res == NULL)
		return (NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(char));
		if (res[i] == NULL)
			return (free_tab(res, len.y), NULL);
		i++;
	}
	return (res);
}

bool	create_map(t_game *game, int fd, char *line)
{
	bool	error;

	game->map_size = get_dimension_maps(fd, line, &error);
	if (game->map_size.x == 0 || game->map_size.y == 0)
		return (print_error("Empty map\n", 1), false);
	if (error == true)
		return (print_error("Unknown element in map\n", 1), false);
	game->map = _init_map(game->map_size);
	if (game->map == NULL)
		return (print_error(NULL, 0), false);
	return (true);
}

bool	parse_map(char *filename, t_game *game, int nb_line)
{
	int		i;
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	line = get_next_line(fd);
	i = -1;
	while (line != NULL && ++i < nb_line)
	{
		free(line);
		line = get_next_line(fd);
	}
	i = -1;
	while (line != NULL && ++i < game->map_size.y)
	{
		remove_end_whitespace(line);
		ft_memcpy(game->map[i], line, ft_strlen(line));
		ft_memset(game->map[i] + (ft_strlen(line)), ' ',
			game->map_size.x - ft_strlen(line));
		free(line);
		line = get_next_line(fd);
	}
	return (free(line), close(fd), true);
}
