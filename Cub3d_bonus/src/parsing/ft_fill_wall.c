/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/05 19:41:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	fill_object_and_doors(t_game *game)
{
	int	x;
	int	y;
	int	cpt_objects;
	int	cpt_doors;

	y = 0;
	if (game->nb_objects != 0)
	{
		game->object_array = ft_calloc(game->nb_objects, sizeof(t_object *));
		if (!game->object_array)
			return (free_map_object(game->map, game->map_size), false);
	}
	if (game->nb_doors != 0)
	{
		game->door_array = ft_calloc(game->nb_doors, sizeof(t_map *));
		if (!game->door_array)
			return (free_map_object(game->map, game->map_size), false);
	}
	cpt_objects = 0;
	cpt_doors = 0;
	while (y < game->map_size.y)
	{
		x = 0;
		while (x < game->map_size.x)
		{
			if ((game->map[y][x].type & OBJECT) == OBJECT)
			{
				game->object_array[cpt_objects] = game->map[y][x].arg;
				game->object_array[cpt_objects]->map_pos = (t_dvector2){x + 0.5f, y + 0.5f};
				cpt_objects++;
			}
			else if ((game->map[y][x].type & DOOR) == DOOR)
			{
				game->door_array[cpt_doors] = &game->map[y][x];
				((t_door *)game->door_array[cpt_doors]->arg)->map_pos = (t_vector2){x, y};
				cpt_doors++;
			}
			x++;
		}
		y++;
	}
	while (cpt_objects < game->nb_objects)
	{
		game->object_array[cpt_objects] = ft_calloc(1, sizeof(t_object));
		if (game->object_array[cpt_objects] == NULL)
			return (print_error(NULL, 0), false);
		game->object_array[cpt_objects]->dist = -1;
		game->object_array[cpt_objects]->map_pos = (t_dvector2){-1, -1};
		cpt_objects++;
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
	int			i;
	int			len;
	bool		error;

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
			if (!get_wall(game, &map[i], line[i]))
				return (false);
		}
		else
		{
			if (!get_none_wall(game, &map[i], error, line[i]))
				return (false);
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
