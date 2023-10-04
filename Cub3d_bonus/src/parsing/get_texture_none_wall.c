/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_none_wall.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:04:49 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 18:21:03 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	_get_default_none_wall(t_game *game, t_map *map)
{
	map->type = NONE;
	map->music = get_music(game->file_music, game->nb_music, map->symbol, e_music);
	if (map->music != NULL)
		map->type |= MUSIC;
	map->narrator = get_narrator(game->file_music, game->nb_music, map->symbol, e_narrator);
	if (map->narrator != NULL)
		map->type |= NARRATOR;
	map->sprite[e_north].index = -1;
	map->sprite[e_east].index = -1;
	map->sprite[e_south].index = -1;
	map->sprite[e_west].index = -1;
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file, map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map->symbol, e_ceiling);
}

static bool	_get_none_wall_object(t_game *game, t_map *map)
{
	game->nb_objects++;
	map->type |= OBJECT;
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file, map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map->symbol, e_ceiling);
	map->sprite[e_object_image] = fill_texture(game->filename, game->nb_file, map->symbol, e_object_entity);
	map->arg = ft_calloc(1, sizeof(t_object));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	return (true);
}

static bool	_get_none_wall_object_interactive(t_game *game, t_map *map)
{
	game->nb_objects++;
	map->type |= OBJECT;
	map->type |= OBJECT_INTERACTIVE;
	map->sprite[e_floor] = fill_texture(game->filename, game->nb_file, map->symbol, e_floor);
	map->sprite[e_ceiling] = fill_texture(game->filename, game->nb_file, map->symbol, e_ceiling);
	map->sprite[e_object_interactive_image] = fill_texture(game->filename, game->nb_file, map->symbol, e_object_interactive);
	map->sprite[e_object_interactive_hand_image] = fill_texture(game->filename, game->nb_file, map->symbol, e_object_interactive_hand);
	map->arg = ft_calloc(1, sizeof(t_object));
	if (map->arg == NULL)
		return (print_error(NULL, 0), false);
	((t_object *)map->arg)->music = get_music(game->file_music, game->nb_music, map->symbol, e_music_object);
	if (((t_object *)map->arg)->music != NULL)
		map->type |= MUSIC_OBJECT;
	return (true);
}

bool	get_none_wall(t_game *game, t_map *map, bool error, char symbol)
{
	if (error == true && !is_sound(game->file_music, game->nb_music, symbol))
		return (print_error("Invalid caracter in the map\n", 1), false);
	_get_default_none_wall(game, map);
	if (is_object(symbol, game->filename, game->nb_file))
	{
		if (!_get_none_wall_object(game, map))
			return (false);
	}
	else if (is_object_interactive(symbol, game->filename, game->nb_file))
	{
		if (!_get_none_wall_object_interactive(game, map))
			return (false);
	}
	return (true);
}
