/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 17:15:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_get_info_mapcell(t_game *game, char c, t_map *map)
{
	bool		error;

	if (is_wall(c, game->filename, game->nb_file, &error))
	{
		if (!get_wall(game, map, c))
			return (false);
	}
	else
		if (!get_none_wall(game, map, error, c))
			return (false);
	return (true);
}

/**
 * @brief initialize the map the symbol find and each texture depending on
 * 		the wall orientation and the ceiling and floor, and if its a wall
 *		It also complete the map with space if the line is smaller than lenght
 * 		 of the map
 * 
 * @param game 
 * @param line free the string
 * @param map 
 * @param map_size 
 * @return bool
 */
bool	ft_fill_wall(t_game *game, char *line, t_map *map, t_vector2 map_size)
{
	int			i;
	int			len;

	i = -1;
	len = ft_strlen(line);
	while (++i < len && i < map_size.x)
	{
		if (line[i] == '\n')
			break ;
		map[i].symbol = line[i];
		if (line[i] == ' ')
			continue ;
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W'
			|| line[i] == 'E')
			line[i] = '0';
		_get_info_mapcell(game, line[i], &map[i]);
	}
	while (i < map_size.x)
		map[i++].symbol = ' ';
	free(line);
	return (true);
}
