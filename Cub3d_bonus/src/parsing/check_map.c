/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:27:20 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/02 14:42:57 by jvigny           ###   ########.fr       */
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

static bool	_check_door(t_map **map, int x, int y, t_vector2 map_size)
{
	if (((map[y - 1][x].type & WALL) == WALL && (map[y + 1][x].type & WALL) == WALL)
		|| ((map[y][x - 1].type & WALL) == WALL && (map[y][x + 1].type & WALL) == WALL))
		return (true);
	return (false);
}

bool	_check_sound(t_game *game, t_map *map_cell)
{
	char *filename;
	void *narrator;

	if ((map_cell->type & MUSIC_OBJECT) == MUSIC_OBJECT
		&& (map_cell->type & OBJECT_INTERACTIVE) != OBJECT_INTERACTIVE)
		return (false);
	if (((map_cell->type & RECEPTACLE) == RECEPTACLE || (map_cell->type & EXIT) == EXIT)
		&& (map_cell->type & MUSIC) == MUSIC)
	{
		filename = get_music(game->file_music, game->nb_music, map_cell->symbol
			, e_music_receptacle_complete);
		if (filename == NULL)
			return (false);
	}
	if (((map_cell->type & RECEPTACLE) == RECEPTACLE || (map_cell->type & EXIT) == EXIT)
		&& (map_cell->type & NARRATOR) == NARRATOR)
	{
		narrator = get_narrator(game->file_music, game->nb_music, map_cell->symbol
			, e_narrator_receptacle_complete);
		if (narrator == NULL)
			return (false);
	}
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
			if (((map[y][x].type & WALL) != WALL || (map[y][x].type & OBJECT) == OBJECT
				|| (map[y][x].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE
				|| (map[y][x].type & RECEPTACLE) == RECEPTACLE) && map[y][x].symbol != ' ')
			{
				if (!_check_sides(map, x, y, game->map_size))
					return (printf("Error : Map not closed %c%c%c\n", map[y][x-1].symbol, map[y][x].symbol, map[y][x+1].symbol), false);
			}
			if ((map[y][x].type & DOOR) == DOOR)
			{
				if (!_check_sides(map, x, y, game->map_size))
					return (printf("Error : Map not closed %c%c%c\n", map[y][x-1].symbol, map[y][x].symbol, map[y][x+1].symbol), false);
				if (!_check_door(map, x, y, game->map_size))
					return (printf("Error : Door at the wrong place\n"), false);
			}
			if (!_check_sound(game, &map[y][x]))
				return (printf("Error : Invalid sound description\n"), false);
			x++;
		}
		y++;
	}
	return (true);
}

void	exit_door_no_receptacle(t_map *exit, int nb_receptacle, t_image *tab_image)
{
	if (nb_receptacle == 0 && exit != NULL)
	{
		exit->sprite[e_door_image].frame = tab_image[exit->sprite[e_door_image].index].nb_total_frame - 1;
		exit->type ^= DOOR_LOCK;
	}
}
