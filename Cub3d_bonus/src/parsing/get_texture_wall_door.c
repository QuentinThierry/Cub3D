/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_wall_door.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:45:44 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 13:17:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_set_exit_door(t_map *map, t_game *game)
{
	map->type |= DOOR_LOCK | EXIT;
	if (game->exit != NULL)
		return (print_error("Multiple exit on the map\n", 1), false);
	game->exit = map;
	return (true);
}

bool	get_wall_door(t_game *game, t_map *map, t_texture *type_door)
{
	map->arg = ft_calloc(1, sizeof(t_door));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	game->nb_doors++;
	map->type |= DOOR;
	if (type_door->orient == e_exit)
	{
		if (!_set_exit_door(map, game))
			return (false);
	}
	if (type_door->orient == e_door_lock)
	{
		map->type |= DOOR_LOCK | RECEPTACLE;
		((t_door *)map->arg)->open_door = type_door->sym_rcp;
		map->sprite[e_door_img + 1] = fill_texture(game->filename,
				game->nb_file, map->symbol, e_door_unlock);
	}
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_ceiling);
	map->sprite[e_door_img] = fill_texture(game->filename, game->nb_file,
			map->symbol, type_door->orient);
	return (true);
}
