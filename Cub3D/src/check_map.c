/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:27:20 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/09 18:26:32 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static bool	_check_sides(char **map, int x, int y, t_vector2 map_size)
{
	if (x == 0 || y == 0 || x == map_size.x - 1 || y == map_size.y - 1)
		return (false);
	if (map[y - 1][x] == ' ' || map[y][x - 1] == ' '
			|| map[y + 1][x] == ' ' || map[y][x + 1] == ' ')
		return (false);
	return (true);
}

bool	check_map(t_game *game)
{
	char	**map;
	int		x;
	int		y;

	map = game->map;
	y = 0;
	while (y < game->map_size.y)
	{
		x = 0;
		while (x < game->map_size.x)
		{
			if (map[y][x] != '0' && map[y][x] != '1' && map[y][x] != 'N'
				&& map[y][x] != 'W' && map[y][x] != 'S' && map[y][x] != 'E'
				&& map[y][x] != ' ')
				return (print_error("Unknown caracter on the map\n", 1), false);
			if (map[y][x] == '0')
			{
				if (!_check_sides(map, x, y, game->map_size))
					return (print_error("Map not closed\n", 1), false);
			}
			x++;
		}
		y++;
	}
	return (true);
}
