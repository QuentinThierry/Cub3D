/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_wall.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 17:57:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 13:16:48 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	get_wall_door(t_game *game, t_map *map, t_texture *type_door);

static void	_get_default_wall(t_game *game, t_map *map)
{
	map->type = WALL;
	map->sprite[e_north] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_north);
	map->sprite[e_east] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_east);
	map->sprite[e_south] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_south);
	map->sprite[e_west] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_west);
	map->sprite[e_floor].index = -1;
	map->sprite[e_ceiling].index = -1;
	map->music = get_music(game->file_music, game->nb_music, map->symbol,
			e_music);
	if (map->music != NULL)
		map->type |= MUSIC;
	map->narrator = get_narrator(game->file_music, game->nb_music, map->symbol,
			e_narrator);
	if (map->narrator != NULL)
		map->type |= NARRATOR;
}

static bool	_get_wall_object(t_game *game, t_map *map)
{
	game->nb_objects++;
	map->type |= OBJECT;
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_ceiling);
	map->sprite[e_object_img] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_object_wall);
	map->arg = ft_calloc(1, sizeof(t_object));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	return (true);
}

static bool	_get_wall_object_interactive(t_game *game, t_map *map)
{
	game->nb_objects += 2;
	map->type |= OBJECT;
	map->type |= OBJECT_INTERACTIVE;
	map->sprite[e_obj_int_img] = fill_texture(game->filename,
			game->nb_file, map->symbol, e_object_interactive);
	map->sprite[e_obj_int_hand_img] = fill_texture(game->filename,
			game->nb_file, map->symbol, e_object_interactive_hand);
	map->sprite[e_obj_int_before_img]
		= fill_texture(game->filename, game->nb_file, map->symbol,
			e_object_interactive_before);
	map->sprite[e_obj_int_after_img] = fill_texture(game->filename,
			game->nb_file, map->symbol, e_object_interactive_after);
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_ceiling);
	map->arg = ft_calloc(1, sizeof(t_object));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	((t_object *)map->arg)->music = get_music(game->file_music, game->nb_music,
			map->symbol, e_music_object);
	if (((t_object *)map->arg)->music != NULL)
		map->type |= MUSIC_OBJECT;
	return (true);
}

static bool	_get_wall_receptacle(t_game *game, t_map *map, char c)
{
	game->nb_objects++;
	game->total_receptacle++;
	map->type |= OBJECT;
	map->type |= RECEPTACLE;
	map->sprite[e_receptacle_empty_img] = fill_texture(game->filename,
			game->nb_file, map->symbol, e_receptacle_empty);
	map->sprite[e_receptacle_full_img] = fill_texture(game->filename,
			game->nb_file, map->symbol, e_receptacle_full);
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file,
			map->symbol, e_ceiling);
	map->arg = ft_calloc(1, sizeof(t_object));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	((t_object *)map->arg)->sym_rcp = c;
	return (true);
}

bool	get_wall(t_game *game, t_map *map, char symbol)
{
	t_texture	type_door;
	char		c;

	_get_default_wall(game, map);
	if (is_door(symbol, game->filename, game->nb_file, &type_door))
	{
		if (!get_wall_door(game, map, &type_door))
			return (false);
	}
	else if (is_object(symbol, game->filename, game->nb_file))
	{
		if (!_get_wall_object(game, map))
			return (false);
	}
	else if (is_object_interactive(symbol, game->filename, game->nb_file))
	{
		if (!_get_wall_object_interactive(game, map))
			return (false);
	}
	else if (is_receptacle(symbol, game->filename, game->nb_file, &c))
	{
		if (!_get_wall_receptacle(game, map, c))
			return (false);
	}
	return (true);
}
