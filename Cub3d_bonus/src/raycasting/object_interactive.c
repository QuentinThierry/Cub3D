/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_interactive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:54:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 13:31:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	take_object(t_game *game, t_player *player, t_map *cell_map,
		t_music_game *music_tab)
{
	if (!(player->has_item == false && (cell_map->type & WALL) != WALL))
		return ;
	player->item = *cell_map;
	((t_object *)player->item.arg)->map_pos = (t_dvector2){-1, -1};
	player->item.type &= ~MUSIC & ~IS_PLAYING_MUSIC;
	if ((cell_map->type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, cell_map, music_tab);
		player->item.type &= ~NARRATOR;
		cell_map->type &= ~NARRATOR;
	}
	if ((cell_map->type & MUSIC_OBJECT) == MUSIC_OBJECT)
		play_music(&player->item, music_tab,
			((t_object *)player->item.arg)->music, IS_PLAYING_OBJECT);
	if ((cell_map->type & IS_PLAYING_OBJECT) == IS_PLAYING_OBJECT)
		update_map_cell_music(&player->item, cell_map, music_tab);
	cell_map->type &= ~MUSIC_OBJECT & ~IS_PLAYING_OBJECT;
	cell_map->symbol = '0';
	cell_map->arg = NULL;
	cell_map->type &= ~OBJECT_INTERACTIVE;
	cell_map->type &= ~OBJECT;
	player->has_item = true;
}

t_vector2	find_pos(t_player *player)
{
	t_dvector2	pos;

	pos.x = sin(player->angle * TO_RADIAN) * M_SQRT2;
	pos.y = -cos(player->angle * TO_RADIAN) * M_SQRT2;
	if (pos.x > 1)
		pos.x = 1;
	else if (pos.x < -1)
		pos.x = -1;
	if (pos.y > 1)
		pos.y = 1;
	else if (pos.y < -1)
		pos.y = -1;
	pos.x += player->f_pos.x;
	pos.y += player->f_pos.y;
	return ((t_vector2){(int)pos.x, (int)pos.y});
}

static void	_set_item_player(t_game *game, t_player *player, t_map *map_cell,
				t_vector2 pos)
{
	player->has_item = true;
	player->item.symbol = map_cell->symbol;
	player->item.type = map_cell->type;
	player->item.type &= ~WALL & ~MUSIC & ~IS_PLAYING_MUSIC;
	player->item.sprite[e_obj_int_img] = map_cell->sprite[e_obj_int_img];
	player->item.sprite[e_obj_int_hand_img]
		= map_cell->sprite[e_obj_int_hand_img];
	player->item.arg = find_empty_object(game);
	((t_object *)player->item.arg)->music = ((t_object *)map_cell->arg)->music;
	((t_object *)player->item.arg)->map_pos = (t_dvector2){pos.x, pos.y};
}

void	take_object_click(t_game *game, t_player *player, t_map **map)
{
	t_vector2	pos;
	t_map		*map_cell;

	pos = find_pos(player);
	map_cell = &map[pos.y][pos.x];
	if ((map_cell->type & RECEPTACLE) == RECEPTACLE
		&& ((map_cell->type & DOOR_LOCK) == DOOR_LOCK
			|| (map_cell->type & OBJECT) == OBJECT))
		play_sound_fail(game, map_cell, game->music_array);
	if (!((map_cell->type & WALL) == WALL && (map_cell->type & OBJECT) == OBJECT
			&& (map_cell->type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE))
		return ;
	_set_item_player(game, player, map_cell, pos);
	map_cell->type &= ~MUSIC_OBJECT & ~IS_PLAYING_OBJECT;
	if ((player->item.type & MUSIC_OBJECT) == MUSIC_OBJECT)
		play_music(&player->item, game->music_array,
			((t_object *)player->item.arg)->music, IS_PLAYING_OBJECT);
	if ((map_cell->type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, map_cell, game->music_array);
		player->item.type &= ~NARRATOR;
		map_cell->type &= ~NARRATOR;
	}
	map_cell->type &= ~OBJECT_INTERACTIVE;
	map_cell->sprite[e_obj_int_img] = map_cell->sprite[e_obj_int_after_img];
}
