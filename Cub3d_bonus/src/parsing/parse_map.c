/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 20:24:43 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 20:26:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/get_next_line.h"

void	remove_end_whitespace(char *str);

static char	*_skip_texture(int fd, int nb_line)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fd);
	while (line != NULL && i < nb_line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	return (line);
}

/**
 * @brief Parse the second part of the file that contain the map with 1 or 0 ...
 * 
 * @param fd fd of th file
 * @param filename name of the file
 * @param game struct to add the map
 * @param nb_line number of line already read with get_next_line in the file open
 * @param line last line read with get_next_line
 * @return true On  Succes
 * @return false ERROR, error already print
 */
bool	parse_map(char *filename, t_game *game, int nb_line)
{
	int		y;
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), free_map(game->map, game->map_size),
			false);
	line = _skip_texture(fd, nb_line);
	y = -1;
	while (line != NULL && ++y < game->map_size.y)
	{
		if (!ft_fill_wall(game, line, game->map[y], game->map_size))
			return (close(fd), free(line), free_map(game->map, game->map_size),
				false);
		line = get_next_line(fd);
	}
	free(line);
	game->map = game->map;
	close(fd);
	if (!fill_object_and_doors(game))
		return (false);
	return (true);
}

/**
 * @brief Get the dimension of the map in the file
 * 
 * @param fd fd to read the map and until the end of the file
 * @param line last line read in the file with gnl
 * @param error set on true if error occurs during the function NOT USE FOR
 * 		THE MOMENT
 * @return t_vector2 width and lenght of the map
 */
static t_vector2	_get_dimension_maps(int fd, char *line, bool *error)
{
	t_vector2	len;

	*error = false;
	len = (t_vector2){0};
	while (line != NULL)
	{
		if (line[0] == '\n')
			break ;
		remove_end_whitespace(line);
		if (len.x < (int)ft_strlen(line))
			len.x = ft_strlen(line);
		len.y++;
		free(line);
		line = get_next_line(fd);
	}
	while (line != NULL)
	{
		if (line[0] != '\n')
			*error = true;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (len);
}

/**
 * @brief calloc the 2D map with the lenght of tha map
 */
static t_map	**_init_map(t_vector2 len)
{
	int		i;
	t_map	**res;

	i = 0;
	res = ft_calloc(len.y, sizeof(t_map *));
	if (res == NULL)
		return (print_error(NULL, 0), NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(t_map));
		if (res[i] == NULL)
			return (print_error(NULL, 0), free_tab((void *)res, len.y), NULL);
		i++;
	}
	return (res);
}

bool	create_map(t_game *game, int fd, char *line)
{
	bool	error;

	game->map_size = _get_dimension_maps(fd, line, &error);
	if (game->map_size.x == 0 || game->map_size.y == 0)
		return (print_error("Empty map\n", 1), false);
	if (error == true)
		return (print_error("Unknown element in map\n", 1), false);
	game->map = _init_map(game->map_size);
	return (true);
}
