/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:27:20 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/13 18:19:32 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_check_sides(t_map **map, int x, int y, t_vector2 map_size)
{

	if (x == 0 || y == 0 || x == map_size.x - 1 || y == map_size.y - 1)
		return (false);
	if (map[y - 1][x].symbol == ' ' || map[y][x - 1].symbol == ' '
			|| map[y + 1][x].symbol == ' ' || map[y][x + 1].symbol == ' ')
		return (false);
	return (true);
}

/**
 * @brief verify if the map is properly close
 * print the Error
 */
bool	check_map(t_game *game)
{
	t_map	**map;
	int		x;
	int		y;

	map = game->map;
	y = 0;
	while (y < game->map_size.y)
	{
		x = 0;
		while (x < game->map_size.x)
		{
			if (((map[y][x].type & WALL) != WALL && map[y][x].symbol != ' ')
				|| (map[y][x].type & DOOR) == DOOR)
			{
				if (!_check_sides(map, x, y, game->map_size))
					return (printf("Error : Map not closed\n"), false);
			}
			x++;
		}
		y++;
	}
	return (true);
}