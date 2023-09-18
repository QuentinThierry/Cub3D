/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/18 16:12:03 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool fill_object_and_doors(t_game *game)
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
			return (false);
	}
	if (game->nb_doors != 0)
	{
		game->door_array = ft_calloc(game->nb_doors, sizeof(t_map *));
		if (!game->door_array)
			return (false);
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
				game->object_array[cpt_objects]->map_pos =
					(t_dvector2){x + 0.5f, y + 0.5f};
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
			return (perror("Error"), false);
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
	int	i;
	int	len;
	enum e_orientation	type_door;
	bool	error;
	char	c;

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
			if (is_door(line[i], game->filename, game->nb_file, &type_door))
			{
				game->nb_doors++;
				map[i].type |= DOOR;
				if (type_door == e_exit)
				{
					map[i].type |= DOOR_LOCK;
					map[i].type |= EXIT;
					if (game->exit != NULL)
						return (printf("Error : Multiple exit on the map\n"), false);
					game->exit = map + i;
				}
				map[i].arg = ft_calloc(1, sizeof(t_door));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].sprite[e_door_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, type_door);
			}
			else if (is_object(line[i], game->filename, game->nb_file))
			{
				game->nb_objects++;
				map[i].type |= OBJECT;
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].sprite[e_object_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_wall);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
			}
			else if (is_object_interactive(line[i], game->filename, game->nb_file))
			{
				game->nb_objects += 2;
				map[i].type |= OBJECT;
				map[i].type |= OBJECT_INTERACTIVE;
				map[i].sprite[e_object_interactive_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive);
				map[i].sprite[e_object_interactive_hand_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive_hand);
				map[i].sprite[e_object_interactive_before_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive_before);
				map[i].sprite[e_object_interactive_after_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive_after);
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
			}
			else if (is_receptacle(line[i], game->filename, game->nb_file, &c))
			{
				game->nb_objects++;
				game->total_receptacle++;
				map[i].type |= OBJECT;
				map[i].type |= RECEPTACLE;
				map[i].sprite[e_receptacle_empty_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_receptacle_empty);
				map[i].sprite[e_receptacle_full_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_receptacle_full);
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
				((t_object *)map[i].arg)->symbol_receptacle = c;
			}
		}
		else
		{
			if (error == true)
				return (printf("Error : invalid caracter in the map\n"), false);
			map[i].type = NONE;
			map[i].sprite[e_north].index = -1;
			map[i].sprite[e_east].index = -1;
			map[i].sprite[e_south].index = -1;
			map[i].sprite[e_west].index = -1;
			map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
			map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
			if (is_object(line[i], game->filename, game->nb_file))
			{
				game->nb_objects++;
				map[i].type |= OBJECT;
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].sprite[e_object_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_entity);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
			}
			else if (is_object_interactive(line[i], game->filename, game->nb_file))
			{
				game->nb_objects++;
				map[i].type |= OBJECT;
				map[i].type |= OBJECT_INTERACTIVE;
				map[i].sprite[e_floor] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_floor);
				map[i].sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_ceiling);
				map[i].sprite[e_object_interactive_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive);
				map[i].sprite[e_object_interactive_hand_image] = fill_texture(game->filename, game->nb_file, map[i].symbol, e_object_interactive_hand);
				map[i].arg = ft_calloc(1, sizeof(t_object));
				if (map[i].arg == NULL)
					return (perror("Error"), false);
			}
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
