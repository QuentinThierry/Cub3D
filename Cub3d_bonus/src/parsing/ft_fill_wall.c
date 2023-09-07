/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/07 17:03:54 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool fill_object_array(t_game *game)
{
	int	x;
	int	y;
	int	i;

	y = 0;
	i = 0;
	if (game->nb_objects == 0)
		return (true);
	game->object_array = ft_calloc(game->nb_objects, sizeof(t_object *));
	if (!game->object_array)
		return (false);
	while (y < game->map_size.y)
	{
		x = 0;
		while (x < game->map_size.x)
		{
			if ((game->map[y][x].type & OBJECT) == OBJECT)
			{
				game->object_array[i] = game->map[y][x].arg;
				game->object_array[i]->map_pos =
					(t_fvector2){x + 0.5f, y + 0.5f};
				i++;
			}
			x++;
		}
		y++;
	}
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
	int	i;
	int	len;
	bool	error;

	i = 0;
	len = ft_strlen(line);
	while (i < len && i < map_size.x)
	{
		if (line[i] == '\n')
			break ;
		map[i].symbol = line[i];
		if (line[i] == ' ')
		{
			i++;
			continue ;
		}
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W' || line[i] == 'E')
			line[i] = '0';
		if (is_wall(line[i], game->filename, game->nb_file, &error))
		{
			map[i].type = WALL;
			map[i].sprite[e_north] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_north);
			map[i].sprite[e_east] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_east);
			map[i].sprite[e_south] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_south);
			map[i].sprite[e_west] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_west);
			map[i].sprite[e_floor].index = -1;
			map[i].sprite[e_ceiling].index = -1;
			if (is_door(line[i], game->filename, game->nb_file))
			{
				map[i].type |= DOOR_CLOSE;
				map[i].arg = ft_calloc(1, sizeof(t_door));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
			}
			else if (is_object(line[i], game->filename, game->nb_file))
			{
				game->nb_objects++;
				map[i].type |= OBJECT;
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].sprite[e_object_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
			}
		}
		else
		{
			if (error == true)
				return (printf("Error : invalid caracter in the map %s\n", line), false);
			map[i].type = NONE;
			map[i].sprite[e_north].index = -1;
			map[i].sprite[e_east].index = -1;
			map[i].sprite[e_south].index = -1;
			map[i].sprite[e_west].index = -1;
			map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
			map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
		}
		i++;
	}
	while (i < map_size.x)
	{
		map[i].symbol = ' ';
		i++;
	}
	free(line);
	return (true);
}
